#pragma once

#include <iostream>

#include "game/game_session.hpp"
#include "game/game_objects/bullet_game_object.hpp"
#include "network/connection_service.hpp"


namespace bt
{
    enum class player_connection_state : unsigned int
    {
        unknown = 0,
        disconnected,
        connecting,
        main_lobby,
        session_lobby,
        game_session,
    };

    class connection_state
    {
    public:
        explicit connection_state(const player_connection_state connection_state)
            : connection_state_(connection_state)
        {
        }

        virtual ~connection_state() = default;

    public:
        virtual void start() = 0;
        virtual player_connection_state handle_event(const sf::Event& event) = 0;
        virtual player_connection_state process_packet(
            command_id_server command_id,
            sf::Uint32 connection_id,
            sf::Uint32 session_id,
            const std::shared_ptr<sf::Packet>& packet) = 0;

    public:
        player_connection_state get_connection_state() const { return connection_state_; }

    private:
        player_connection_state connection_state_;
    };


    //TODO:: handle lost connection and reconnect to session if exists
    class player_disconnected_state final : public connection_state
    {
    public:
        player_disconnected_state()
            : connection_state(player_connection_state::disconnected)
        {
        }

        virtual ~player_disconnected_state() override = default;

    public:
        virtual void start() override
        {
        }

        virtual player_connection_state handle_event(const sf::Event& event) override
        {
            return get_connection_state();
        }

        virtual player_connection_state process_packet(
            const command_id_server command_id,
            const sf::Uint32 connection_id,
            const sf::Uint32 session_id,
            const std::shared_ptr<sf::Packet>& packet) override
        {
            return get_connection_state();
        }
    };

    class player_connecting_state final : public connection_state
    {
    public:
        explicit player_connecting_state(const std::shared_ptr<game_session>& game_session)
            : connection_state(player_connection_state::connecting), game_session_{ game_session }
        {
        }

        virtual ~player_connecting_state() override = default;
    public:
        virtual void start() override
        {
            std::cout << "connecting..." << std::endl;
        }

        virtual player_connection_state handle_event(const sf::Event& event) override
        {
            return get_connection_state();
        }

        virtual player_connection_state process_packet(
            const command_id_server command_id,
            const sf::Uint32 connection_id,
            const sf::Uint32 session_id,
            const std::shared_ptr<sf::Packet>& packet) override
        {
            if (command_id == command_id_server::connection_established)
            {

                bt::uuid player_id;
                *packet >> player_id;
                game_session_->player_id_ = player_id;
                std::cout << "connected" << std::endl;
                return player_connection_state::main_lobby;
            }

            return get_connection_state();
        }

    private:
        std::shared_ptr<game_session> game_session_{ nullptr };
    };

    class main_lobby_state final : public connection_state
    {
    public:
        explicit main_lobby_state(std::shared_ptr<bt::connection_service> connection_service)
            : connection_state(player_connection_state::main_lobby), connection_service_(std::move(connection_service))
        {
        }

        virtual ~main_lobby_state() override = default;

    public:
        virtual void start() override
        {
        }

        virtual player_connection_state handle_event(const sf::Event& event) override
        {
            //TODO:: don't allow to repeat it until server response
            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::C)
                {
                    std::cout << "creating session... " << std::endl;
                    connection_service_->send(client_command{ command_id_client::create_session });
                }
                else if (event.key.code == sf::Keyboard::J)
                {
                    std::cout << "joining to session... id: " << 2 << std::endl;
                    connection_service_->send(join_session_command{ 2 });

                    return bt::player_connection_state::session_lobby;
                }
            }

            return get_connection_state();
        }

        virtual player_connection_state process_packet(
            const command_id_server command_id,
            const sf::Uint32 connection_id,
            const sf::Uint32 session_id,
            const std::shared_ptr<sf::Packet>& packet) override
        {
            if (command_id == command_id_server::session_created)
            {
                sf::Uint32 current_session;
                *packet >> current_session;
                std::cout << "session created, id: " << current_session << std::endl;

                //TODO:: parse session_id and connection_id in connection layer
                connection_service_->get_connection().set_session_id(current_session);
                std::cout << "joining to session... id: " << current_session << std::endl;
                connection_service_->send(join_session_command{ current_session });

                return bt::player_connection_state::session_lobby;
            }

            return get_connection_state();
        }

    private:
        std::shared_ptr<bt::connection_service> connection_service_{ nullptr };
    };

    class session_lobby_state final : public connection_state
    {
    public:
        session_lobby_state(
            std::shared_ptr<bt::texture_warehouse> texture_warehouse,
            std::weak_ptr<bt::physics_body_factory> physics_body_factory,
            std::function<void(const std::shared_ptr<bt::player_game_object>& player)> player_created_callback
        )
            : connection_state(player_connection_state::session_lobby), texture_warehouse_(std::move(texture_warehouse)),
              physics_body_factory_(std::move(physics_body_factory)),
              player_created_callback_(std::move(player_created_callback))
        {
        }

        virtual ~session_lobby_state() override = default;

    public:
        virtual void start() override
        {
        }

        virtual player_connection_state handle_event(const sf::Event& event) override
        {
            return get_connection_state();
        }

        virtual player_connection_state process_packet(
            const command_id_server command_id,
            const sf::Uint32 connection_id,
            const sf::Uint32 session_id,
            const std::shared_ptr<sf::Packet>& packet) override
        {
            if (command_id == command_id_server::player_joined_to_session)
            {
                std::cout << "joined to session, id: " << session_id << std::endl;
                //TODO:: add player to game session and display in session lobby connected players
            }
            else if (command_id == command_id_server::session_started)
            {
                sf::Uint32 started_session_id;
                *packet >> started_session_id;
                std::cout << "game session started, id: " << session_id << std::endl;
                //TODO:: start game world rendering and physics simulation
                sf::Uint32 game_objects_count;
                *packet >> game_objects_count;
                for (sf::Uint32 i = 0; i < game_objects_count; ++i)
                {
                    bt::uuid game_object_id;
                    *packet >> game_object_id;
                    bt::game_object_frame_restorer_packet restorer_packet{ packet };
                    auto player = std::make_shared<bt::player_game_object>(
                        game_object_id,
                        texture_warehouse_->load_pack(bt::textures_pack_id::tank),
                        physics_body_factory_
                    );
                    player->initialize();
                    player->restore_from_frame(restorer_packet);
                    player_created_callback_(player);
                }

                return bt::player_connection_state::game_session;
            }
            return get_connection_state();
        }

    private:
        std::shared_ptr<bt::texture_warehouse> texture_warehouse_;
        std::weak_ptr<bt::physics_body_factory> physics_body_factory_;
        std::function<void(const std::shared_ptr<bt::player_game_object>& player)> player_created_callback_;
    };

    class game_session_state final : public connection_state
    {
    public:
        game_session_state(
            std::shared_ptr<bt::connection_service> connection_service,
            std::shared_ptr<bt::texture_warehouse> texture_warehouse,
            std::weak_ptr<bt::physics_body_factory> physics_body_factory,
            std::function<void(const std::shared_ptr<bt::game_object>& game_object)> game_object_created_callback,
            std::function<void(const bt::uuid game_object_id)> game_object_deleted_callback,
            std::function<void(const bt::uuid game_object_id, const bt::game_object_frame_restorer_packet& restorer_packet)> restore_game_object_callback
        )
            : connection_state(player_connection_state::game_session),
              physics_body_factory_(std::move(physics_body_factory)), texture_warehouse_(std::move(texture_warehouse)),
              game_object_created_callback_(std::move(game_object_created_callback)),
              game_object_deleted_callback_(std::move(game_object_deleted_callback)),
              restore_game_object_callback_(std::move(restore_game_object_callback)),
              connection_service_(std::move(connection_service))
        {
        }
        virtual ~game_session_state() override = default;

    public:
        virtual void start() override
        {
            bullet_creation_time_ = std::chrono::high_resolution_clock::now();
        }

        virtual player_connection_state handle_event(const sf::Event& event) override
        {
            //TODO:: start move object immediately after key pressed. don't wait server to move 
            //player_action tank_move_action{};
            //player_action tank_rotate_action{};
            //player_action tower_rotate_action{};
            //bool tank_fire{ false };
            // ///////////////////////////

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            {
                connection_service_->send(player_action_command{ player_action::move_forward });
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            {
                connection_service_->send(player_action_command{ player_action::move_backward });
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            {
                connection_service_->send(player_action_command{ player_action::turn_left });
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            {
                connection_service_->send(player_action_command{ player_action::turn_right });
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            {
                connection_service_->send(player_action_command{ player_action::turn_turret_left });
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            {
                connection_service_->send(player_action_command{ player_action::turn_turret_right });
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
            {
                const auto bullet_current_time = std::chrono::high_resolution_clock::now();
                if (std::chrono::duration_cast<std::chrono::milliseconds>(bullet_current_time - bullet_creation_time_).count() >= 1000)
                {
                    bullet_creation_time_ = bullet_current_time;
                    connection_service_->send(player_action_command{ player_action::turret_fire });
                }
            }

            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::S)
                {
                    connection_service_->send(player_action_command{ player_action::stop_move });
                }

                if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::D)
                {
                    connection_service_->send(player_action_command{ player_action::stop_turn });
                }

                if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right)
                {
                    connection_service_->send(player_action_command{ player_action::stop_turn_turret });
                }

                if (event.key.code == sf::Keyboard::Space)
                {
                    connection_service_->send(player_action_command{ player_action::turret_stop_fire });
                }
            }

            return get_connection_state();
        }

        virtual player_connection_state process_packet(
            const command_id_server command_id,
            const sf::Uint32 connection_id,
            const sf::Uint32 session_id,
            const std::shared_ptr<sf::Packet>& packet) override
        {
            if (command_id == command_id_server::update_game_frame)
            {
                sf::Uint32 game_objects_count;
                *packet >> game_objects_count;
                for (sf::Uint32 i = 0; i < game_objects_count; ++i)
                {
                    bt::uuid game_object_id;
                    *packet >> game_object_id;

                    bt::game_object_frame_restorer_packet restorer_packet{ packet };
                    restore_game_object_callback_(game_object_id, restorer_packet);
                }
            }
            else if (command_id == command_id_server::player_shoot)
            {
                bt::uuid player_id;
                *packet >> player_id;
                bt::uuid bullet_id;
                *packet >> bullet_id;
                const bt::game_object_frame_restorer_packet restorer_packet{ packet };
                const auto bullet = std::make_shared<bt::bullet_game_object>(
                    bullet_id,
                    physics_body_factory_,
                    texture_warehouse_->load_texture(bt::texture_id::bullet)
                );
                bullet->initialize();
                bullet->restore_from_frame(restorer_packet);
                game_object_created_callback_(bullet);
                
            }
            else if (command_id == command_id_server::delete_game_object)
            {
                bt::uuid game_object_id;
                *packet >> game_object_id;
                game_object_deleted_callback_(game_object_id);
            }

            return get_connection_state();
        }

    private:
        std::weak_ptr<bt::physics_body_factory> physics_body_factory_;
        std::shared_ptr<bt::texture_warehouse> texture_warehouse_;
        std::function<void(const std::shared_ptr<bt::game_object>& game_object)> game_object_created_callback_;
        std::function<void(const bt::uuid game_object_id)> game_object_deleted_callback_;
        std::function<void(const bt::uuid game_object_id, const bt::game_object_frame_restorer_packet& restorer_packet)> restore_game_object_callback_;
        std::shared_ptr<bt::connection_service> connection_service_;

        std::chrono::time_point<std::chrono::steady_clock> bullet_creation_time_{};
    };

    class connection_state_manager
    {
    public:
        explicit connection_state_manager(
            std::shared_ptr<game_session> game_session,
            std::shared_ptr<bt::texture_warehouse> texture_warehouse,
            std::weak_ptr<bt::physics_body_factory> physics_body_factory,
            std::function<void(const std::shared_ptr<bt::player_game_object>& player)> player_created_callback,
            std::function<void(const std::shared_ptr<bt::game_object>& game_object)> game_object_created_callback,
            std::function<void(const bt::uuid game_object_id)> game_object_deleted_callback,
            std::function<void(const bt::uuid game_object_id, const bt::game_object_frame_restorer_packet& restorer_packet)> restore_game_object_callback
        )
        //TODO:: add signals or events library to substitute callbacks
            : game_session_(std::move(game_session)), texture_warehouse_(std::move(texture_warehouse)),
              physics_body_factory_(std::move(physics_body_factory)),
              player_created_callback_(std::move(player_created_callback)),
              game_object_created_callback_(std::move(game_object_created_callback)),
              game_object_deleted_callback_(std::move(game_object_deleted_callback)),
              restore_game_object_callback_(std::move(restore_game_object_callback))
        {
        }

        ~connection_state_manager() = default;

    public:
        player_connection_state get_connection_state() const
        {
            if (current_state_ == nullptr)
            {
                return player_connection_state::unknown;
            }

            return current_state_->get_connection_state();
        }

        void start()
        {
            if (current_state_ != nullptr)
            {
                return;
            }
            commands_in_ = std::make_shared<ts_deque<connection_command_in>>();
            connection_service_ = std::make_unique<bt::connection_service>(commands_in_);

            current_state_ = std::make_unique<player_connecting_state>(game_session_);
            current_state_->start();
            connection_service_->get_connection().connect({ "localhost" }, 52000);
        }

        void update(const float delta_time)
        {
            if (current_state_ == nullptr)
            {
                return;
            }

            connection_service_->get_connection().update();

            if (commands_in_->empty())
            {
                return;
            }

            auto [connection_id, session_id, packet_ref] = commands_in_->pop_front();
            sf::Uint32 command_id;
            *packet_ref >> command_id;

            const auto server_command_id = static_cast<command_id_server>(command_id);

            const auto state = current_state_->process_packet(server_command_id, connection_id, session_id, packet_ref);
            change_state(state);
        }

        void handle_event(const sf::Event& event)
        {
            const auto state = current_state_->handle_event(event);
            change_state(state);
        }

    private:
        void change_state(const player_connection_state new_state)
        {
            if (new_state == current_state_->get_connection_state())
            {
                return;
            }
            //TODO:: move to factory or DI
            switch (new_state)
            {
            case player_connection_state::disconnected:
                current_state_ = std::make_unique<player_disconnected_state>();
                break;
            case player_connection_state::connecting:
                current_state_ = std::make_unique<player_connecting_state>(game_session_);
                break;
            case player_connection_state::main_lobby:
                current_state_ = std::make_unique<main_lobby_state>(connection_service_);
                break;
            case player_connection_state::session_lobby:
                current_state_ = std::make_unique<session_lobby_state>(
                    texture_warehouse_,
                    physics_body_factory_,
                    player_created_callback_
                );
                break;
            case player_connection_state::game_session:
                current_state_ = std::make_unique<game_session_state>(
                    connection_service_,
                    texture_warehouse_,
                    physics_body_factory_,
                    game_object_created_callback_,
                    game_object_deleted_callback_,
                    restore_game_object_callback_
                );
                break;
            case player_connection_state::unknown:
                //TODO:: handle state switch error
                break;
            }

            current_state_->start();
        }

    private:
        std::unique_ptr<connection_state> current_state_{ nullptr };

        std::shared_ptr<ts_deque<connection_command_in>> commands_in_{ nullptr };
        std::shared_ptr<bt::connection_service> connection_service_{ nullptr };

        std::shared_ptr<game_session> game_session_{ nullptr };
        std::shared_ptr<bt::texture_warehouse> texture_warehouse_;
        std::weak_ptr<bt::physics_body_factory> physics_body_factory_;
        std::function<void(const std::shared_ptr<bt::player_game_object>& player)> player_created_callback_;
        std::function<void(const std::shared_ptr<bt::game_object>& game_object)> game_object_created_callback_;
        std::function<void(const bt::uuid game_object_id)> game_object_deleted_callback_;
        std::function<void(const bt::uuid game_object_id, const bt::game_object_frame_restorer_packet& restorer_packet)> restore_game_object_callback_;
    };
}
