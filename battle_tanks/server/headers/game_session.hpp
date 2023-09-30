#pragma once

#include <unordered_set>

#include "SFML/System/Clock.hpp"
#include "box2d/b2_world.h"

#include "game/entity/game_object_entity.hpp"
#include "game/entity/player_game_object_entity.hpp"
#include "game/game_objects/game_object_frame_restorer_packet.hpp"

#include "network/commands.hpp"
#include "physics/collision_listener.hpp"


enum class game_session_type : sf::Uint32
{
    unknown = 0,
    pvp_1x1,
    team_2x2,
    deathmatch_4,
};

enum class game_session_state : unsigned
{
    unknown = 0,
    waiting_for_players,
    starting,
    game_play_progress,
    finished
};

class game_session
{
public:
    explicit game_session(const sf::Uint32 session_id, const std::shared_ptr<ts_deque<connection_command_out>>& commands_out)
        : commands_out_{ commands_out }, session_id_{ session_id }
    {
        game_objects_ = std::unordered_map<sf::Uint32, std::unique_ptr<bt::game_object_entity>>{};
        physics_world_.SetAllowSleeping(true);
        //physics_world.SetContinuousPhysics(true);;
        physics_world_.SetContactListener(&contact_listener_);

        contact_listener_.add_collision_handler([this](bt::game_object_b2d_link game_object_a, bt::game_object_b2d_link game_object_b)
            {
                if (game_object_a.type == bt::game_object_type::bullet)
                {
                    objects_to_delete_.insert(game_object_a.id);
                }
                else if (game_object_b.type == bt::game_object_type::bullet)
                {
                    objects_to_delete_.insert(game_object_b.id);
                }
            });

        state_ = game_session_state::waiting_for_players;
    }

    ~game_session()
    {
        state_ = game_session_state::finished;
        if (thread_game_loop_.joinable())
        {
            thread_game_loop_.join();
        }
    }

    sf::Uint32 get_session_id() const
    {
        return session_id_;
    }

    game_session_state get_state() const
    {
        return state_;
    }

    const std::vector<sf::Uint32>& get_players_ids() const
    {
        return players_ids_;
    }

    bool join_player(sf::Uint32 player_id);

    void move_player(const sf::Uint32 player_id, const player_action action)
    {
        std::lock_guard lock{ mutex_game_loop_ };
        if (const auto controller = get_player_action_controller(player_id))
        {
            controller->set_move_action(action);
        }
    }

    void rotate_player(const sf::Uint32 player_id, const player_action action)
    {
        std::lock_guard lock{ mutex_game_loop_ };
        if (const auto controller = get_player_action_controller(player_id))
        {
            controller->set_rotate_action(action);
        }
    }

    void rotate_player_turret(const sf::Uint32 player_id, const player_action action)
    {
        std::lock_guard lock{ mutex_game_loop_ };
        if (const auto controller = get_player_action_controller(player_id))
        {
            controller->set_turret_rotate_action(action);
        }
    }

private:
    void update();

    void delete_game_objects();
    void send_game_session_frame(const float delta_time);


    std::shared_ptr<bt::player_action_controller> get_player_action_controller(const sf::Uint32 player_id)
    {
        //std::lock_guard lock{ mutex_game_loop_ };
        if (const auto action_controller = players_action_controllers_.find(player_id); action_controller != players_action_controllers_.end())
        {
            if (action_controller->second.expired())
            {
                players_action_controllers_.erase(player_id);
            }
            else
            {
                return action_controller->second.lock();
            }
        }
        return nullptr;
    }

private:
    std::shared_ptr<ts_deque<connection_command_out>> commands_out_{ nullptr };

    std::thread thread_game_loop_;
    std::mutex mutex_game_loop_;

    sf::Uint32 session_id_{};
    game_session_state state_{};

    b2World physics_world_ = b2World{ b2Vec2{ 0.0f, 0.0f } };
    bt::collision_listener contact_listener_{};
    bt::physics_body_factory physics_body_factory_{ &physics_world_ };

    std::unordered_set<bt::uuid> objects_to_delete_{};
    std::unordered_map<sf::Uint32, std::unique_ptr<bt::game_object_entity>> game_objects_{};
    std::unordered_map<sf::Uint32, std::weak_ptr<bt::player_action_controller>> players_action_controllers_{};

    std::vector<sf::Uint32> players_ids_{};
};

void game_session::update()
{
    sf::Clock clock;
    constexpr int count_down_to_start_milliseconds{ 3000 };
    int current_count_down_time{ count_down_to_start_milliseconds };
    while (state_ != game_session_state::finished)
    {
        if (state_ == game_session_state::starting)
        {
            current_count_down_time -= clock.restart().asMilliseconds();
            if (current_count_down_time <= 0)
            {
                state_ = game_session_state::game_play_progress;

                auto command = std::make_unique<session_started_command>();
                command->session_id = session_id_;

                for (auto player_id : players_ids_)
                {
                    command->game_objects.push_back(game_objects_.at(player_id)->get_frame());
                }

                commands_out_->push_back({ players_ids_, std::move(command) });
                continue;
            }
        }

        if (state_ == game_session_state::game_play_progress)
        {
            std::lock_guard lock{ mutex_game_loop_ };
            delete_game_objects();
            const float delta_time = clock.restart().asSeconds();
            physics_world_.Step(delta_time, 2, 1);
            send_game_session_frame(delta_time);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 60));
    }
}

void game_session::delete_game_objects()
{
    for (const auto& game_object_id : objects_to_delete_)
    {
        game_objects_.erase(game_object_id);
        commands_out_->push_back({
            players_ids_,
            std::make_unique<deleted_game_object_command>(game_object_id)
        });
    }
    objects_to_delete_.clear();
}

void game_session::send_game_session_frame(const float delta_time)
{
    auto command = std::make_unique<game_frame_command>();
    for (const auto& game_object : game_objects_ | std::views::values)
    {
        game_object->update(delta_time);
        command->game_objects.push_back(game_object->get_frame());
    }
    commands_out_->push_back({ players_ids_, std::move(command) });
}

bool game_session::join_player(sf::Uint32 player_id)
{
    if (state_ != game_session_state::waiting_for_players)
    {
        return false;
    }


    std::lock_guard lock{ mutex_game_loop_ };

    auto player_entity = std::make_unique<bt::player_game_object_entity>(player_id, physics_body_factory_);
    player_entity->create_phy_body();


    //TODO:: new player state configuration
    const auto packet = std::make_shared<sf::Packet>();
    player_game_object_frame frame{};
    if (players_ids_.empty())
    {
        frame.position = { 10.0f, 10.f };
    }
    else
    {
        frame.position = { 40.0f, 40.f };
    }
    frame.write_to_packet(*packet);
    sf::Uint32 id;
    *packet >> id;
    const bt::game_object_frame_restorer_packet restorer{ packet };
    player_entity->restore_frame(restorer);
    /////////////////////////////////////////

    players_action_controllers_.emplace(player_id, player_entity->get_action_controller());
    game_objects_.emplace(player_id, std::move(player_entity));
    players_ids_.push_back(player_id);

    auto joined_command = std::make_unique<player_joined_to_session_command>();
    joined_command->session_id = session_id_;
    joined_command->player_id = player_id;
    commands_out_->push_back({ players_ids_, std::move(joined_command) });

    if (players_ids_.size() == 2)
    {
        state_ = game_session_state::starting;
        thread_game_loop_ = std::thread{ &game_session::update, this };
    }

    return true;
}