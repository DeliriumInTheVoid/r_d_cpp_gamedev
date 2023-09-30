#pragma once

#include <memory>
#include <unordered_map>

#include "SFML/Config.hpp"
#include "SFML/Network/Packet.hpp"

#include "network/network_server.hpp"
#include "game_session.hpp"


class game_server
{
public:
    game_server()
    {
        commands_out_ = std::make_shared<ts_deque<connection_command_out>>();
    }

    ~game_server()
    {
        is_running_ = false;
        if (thread_commands_send_.joinable())
        {
            thread_commands_send_.join();
        }
    }

public:
    bool start(const unsigned short port);
    [[noreturn]] void run_infinity_loop(const size_t max_messages_in, const size_t max_messages_out, const bool wait_in);

private:
    void send_commands();

    void create_game_session(const sf::Uint32 connection_id);
    void join_game_session(const sf::Uint32 connection_id, const sf::Uint32 connection_session_id, const sf::Uint32 join_session_id);

private:
    bool is_running_{ false };
    size_t max_messages_out_{};
    std::unordered_map<sf::Uint32, std::unique_ptr<game_session>> sessions_{};
    std::unique_ptr<network_server> network_server_{};
    std::shared_ptr<ts_deque<connection_command_out>> commands_out_{ nullptr };
    std::thread thread_commands_send_;

    std::unordered_map<sf::Uint32, sf::Uint32> connection_to_session_link_{};
};

bool game_server::start(const unsigned short port)
{
    network_server_ = std::make_unique<network_server>(port);

    network_server_->register_client_command_handler(command_id_client::create_session,
        [this](const sf::Uint32 connection_id, sf::Packet& packet)
        {
            create_game_session(connection_id);
        });

    network_server_->register_client_command_handler(command_id_client::join_session,
        [this](const sf::Uint32 connection_id, sf::Packet& packet)
        {
            sf::Uint32 join_session_id;
            packet >> join_session_id;

            sf::Uint32 current_session_id{};
            if (connection_to_session_link_.contains(connection_id))
            {
                current_session_id = connection_to_session_link_.at(connection_id);
            }
            join_game_session(connection_id, current_session_id, join_session_id);
        });

    network_server_->register_client_command_handler(command_id_client::player_action,
        [this](const sf::Uint32 connection_id, sf::Packet& packet)
        {
            sf::Uint32 action_rough;
            packet >> action_rough;

            const auto action = static_cast<player_action>(action_rough);

            sf::Uint32 session_id{};
            if (connection_to_session_link_.contains(connection_id))
            {
                session_id = connection_to_session_link_.at(connection_id);
            }
            //TODO:: else send error to client

            if (const auto session = sessions_.find(session_id); session != sessions_.end())
            {
                switch(action)
                {
                case player_action::move_forward:
                case player_action::move_backward:
                case player_action::stop_move:
                    session->second->move_player(connection_id, action);
	                break;
                case player_action::turn_left:
                case player_action::turn_right:
                case player_action::stop_turn:
                    session->second->rotate_player(connection_id, action);
	                break;
                case player_action::turn_turret_left:
                case player_action::turn_turret_right:
                case player_action::stop_turn_turret:
                    session->second->rotate_player_turret(connection_id, action);
	                break;
                case player_action::turret_fire:
                case player_action::turret_stop_fire:

	                break;
                case player_action::unknown:
                default:
                    //TODO:: send error to client
                	;
                }
            }

        });

    network_server_->register_client_command_handler(command_id_client::lost_connection,
        [this](const sf::Uint32 connection_id, sf::Packet& packet)
        {
            //TODO:: remove player from session and remove session if it has one or less players
            connection_to_session_link_.erase(connection_id);
        });

    return network_server_->start();
}

[[noreturn]] void game_server::run_infinity_loop(const size_t max_messages_in, const size_t max_messages_out, const bool wait_in)
{
    is_running_ = true;
    max_messages_out_ = max_messages_out;
    thread_commands_send_ = std::thread(&game_server::send_commands, this);
    while (true)
    {
        network_server_->update(max_messages_in, wait_in);
    }
}

void game_server::send_commands()
{
    while (is_running_)
    {
        commands_out_->wait();

        size_t message_count = 0;
        while (message_count < max_messages_out_ && !commands_out_->empty())
        {
            auto [connection_ids, command] = commands_out_->pop_front();
            network_server_->send_command(connection_ids, command);
            message_count++;
        }
    }
}

void game_server::create_game_session(const sf::Uint32 connection_id)
{
    if (const auto session_pair = sessions_.find(connection_id); session_pair == sessions_.end())
    {
        const auto session_id = bt::generate_uuid();
        sessions_[session_id] = std::make_unique<game_session>(session_id, commands_out_);

        auto command = std::make_unique<session_created_command>();
        command->session_id = session_id;

        network_server_->send_command(connection_id, std::move(command));

        //sessions_[session_id]->join_player(connection_id);
    }
}

void game_server::join_game_session(const sf::Uint32 connection_id, const sf::Uint32 connection_session_id, const sf::Uint32 join_session_id)
{
    //TODO:: player has no active sessions
    if (connection_session_id == 0)
    {
        if (const auto session_pair = sessions_.find(join_session_id); session_pair != sessions_.end())
        {
            const auto& session = session_pair->second;

            if (session->get_state() == game_session_state::waiting_for_players)
            {
                if (session->join_player(connection_id))
                {
                    connection_to_session_link_[connection_id] = join_session_id;
                }
                // TODO:: else{} send error
            }
        }

    }
}
