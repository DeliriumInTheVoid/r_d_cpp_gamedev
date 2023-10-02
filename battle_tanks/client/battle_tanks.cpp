#include <iostream>
#include <chrono>
#include <memory>

#include "SFML/System/Clock.hpp"
#include "SFML/Graphics.hpp"
#include <SFML/Network.hpp>
#include <box2d/box2d.h>

#include "utils/uuid.hpp"
#include "game/game_objects/player_game_object.hpp"
#include "game/sfml_game.hpp"
#include "game/game_objects/bullet_game_object.hpp"
#include "physics/collision_listener.hpp"
#include "network/client_connection.hpp"
#include "network/connection_service.hpp"
#include "network/player_actions.hpp"
#include "game/game_objects/forest_map_game_object.hpp"
#include "game/game_objects/game_object_frame_restorer_packet.hpp"

enum class player_game_state : unsigned int
{
    unknown = 0,
    disconnected,
    connecting,
    main_lobby,
    session_lobby,
    game_session,
};

int main()
{
    auto player_state = player_game_state::disconnected;

    auto commands_in = std::make_shared<ts_deque<connection_command_in>>();
    auto connection_service = std::make_unique<bt::connection_service>(commands_in);
    connection_service->get_connection().connect({ "localhost" }, 52000);

    player_state = player_game_state::connecting;
    std::cout << "connecting..." << std::endl;


    constexpr float pixels_per_meters{ 10.0f }; // 1 meter = 10 pixels
    const sf::Vector2u game_field_size{ 1024, 768 };

    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(game_field_size.x, game_field_size.y), "Battle Tanks");
    //window->setFramerateLimit(20);

    sf::Clock clock;

    std::unique_ptr<bt::sfml_game> game = std::make_unique<bt::sfml_game>(game_field_size, window, pixels_per_meters);

    bt::texture_warehouse texture_warehouse{};

    auto map_atlas = bt::atlas_data{
        bt::atlas_id::map_forest_tile_set,
        "game_data/atlases/map_forest_tileset.png",
        {
            { bt::texture_id::grass_light_tl, sf::IntRect{1,1,16,16} },
            { bt::texture_id::grass_light_tm, sf::IntRect{18,1,16,16} },
            { bt::texture_id::grass_light_tr, sf::IntRect{35,1,16,16} },

            { bt::texture_id::grass_light_lm, sf::IntRect{1,18,16,16} },
            { bt::texture_id::grass_light_m, sf::IntRect{18,18,16,16} },
            { bt::texture_id::grass_light_rm, sf::IntRect{35,18,16,16} },

            { bt::texture_id::grass_light_bl, sf::IntRect{1,35,16,16} },
            { bt::texture_id::grass_light_bm, sf::IntRect{18,35,16,16} },
            { bt::texture_id::grass_light_br, sf::IntRect{35,35,16,16} },

            { bt::texture_id::grass_light_dec_1, sf::IntRect{52,1,16,16} },
            { bt::texture_id::grass_light_dec_2, sf::IntRect{69,1,16,16} },
            { bt::texture_id::grass_light_dec_3, sf::IntRect{86,1,16,16} },

            { bt::texture_id::grass_light_dec_4, sf::IntRect{52,18,16,16} },
            { bt::texture_id::grass_light_dec_5, sf::IntRect{69,18,16,16} },
            { bt::texture_id::grass_light_dec_6, sf::IntRect{86,18,16,16} },

            { bt::texture_id::grass_light_dec_7, sf::IntRect{52,35,16,16} },
            { bt::texture_id::grass_light_dec_8, sf::IntRect{69,35,16,16} },
            { bt::texture_id::grass_light_dec_9, sf::IntRect{86,35,16,16} },

            { bt::texture_id::grass_dark_tl, sf::IntRect{1,52,16,16} },
            { bt::texture_id::grass_dark_tm, sf::IntRect{18,52,16,16} },
            { bt::texture_id::grass_dark_tr, sf::IntRect{35,52,16,16} },

            { bt::texture_id::grass_dark_lm, sf::IntRect{1,69,16,16} },
            { bt::texture_id::grass_dark_m, sf::IntRect{18,69,16,16} },
            { bt::texture_id::grass_dark_rm, sf::IntRect{35,69,16,16} },

            { bt::texture_id::grass_dark_bl, sf::IntRect{1,86,16,16} },
            { bt::texture_id::grass_dark_bm, sf::IntRect{18,86,16,16} },
            { bt::texture_id::grass_dark_br, sf::IntRect{35,86,16,16} },
        }
    };

    texture_warehouse.pre_load_atlas(map_atlas);

    bool tank_fire{ false };
    //TODO:: start move object immediately after key pressed. don't wait server to move 
    player_action tank_move_action{};
    player_action tank_rotate_action{};
    player_action tower_rotate_action{};

    auto forest_game_map = std::make_shared<bt::forest_map_game_object>(
        0,
        texture_warehouse.load_pack(bt::textures_pack_id::map_forest),
        sf::Vector2u{ 1024, 768 },
        game->get_physics_body_factory()
    );
    forest_game_map->initialize();
    game->add_game_object(forest_game_map);

    auto rock_texture_render_data = std::make_shared<bt::texture_holder>("game_data/atlases/rock_1.png");
    forest_game_map->create_rock({ 200, 200 }, rock_texture_render_data);

    auto bullet_creation_time = std::chrono::high_resolution_clock::now();
    while (window->isOpen())
    {
        float delta_time = clock.restart().asSeconds();

        sf::Uint32 current_session{};

        //<poll server commands>
        connection_service->get_connection().update();
        if (!commands_in->empty())
        {
            auto [connection_id, session_id, packet_ref] = commands_in->pop_front();
            sf::Uint32 command_id;
            *packet_ref >> command_id;

            auto server_command_id = static_cast<command_id_server>(command_id);

            switch (server_command_id)
            {
            case command_id_server::connected_to_game:
            {
                if (player_state == player_game_state::connecting)
                {
                    player_state = player_game_state::main_lobby;
                    std::cout << "connected" << std::endl;
                }
                break;
            }
            case command_id_server::session_created:
            {
                if (player_state == player_game_state::main_lobby)
                {
                    player_state = player_game_state::session_lobby;
                    *packet_ref >> current_session;
                    std::cout << "session created, id: " << current_session << std::endl;

                    //TODO:: parse session_id and connection_id in connection layer
                    connection_service->get_connection().set_session_id(current_session);
                    std::cout << "joining to session... id: " << current_session << std::endl;
                    connection_service->send(join_session_command{ current_session });
                }
                break;
            }
            case command_id_server::player_joined_to_session:
            {
                if (player_state == player_game_state::session_lobby)
                {
                    std::cout << "joined to session, id: " << session_id << std::endl;
                    //TODO:: add player to game session and display in session lobby connected players
                }
                break;
            }
            case command_id_server::session_started:
            {
                if (player_state == player_game_state::session_lobby)
                {
                    player_state = player_game_state::game_session;
                    std::cout << "game session started, id: " << session_id << std::endl;
                    *packet_ref >> session_id;
                    //TODO:: start game world rendering and physics simulation
                    sf::Uint32 game_objects_count;
                    *packet_ref >> game_objects_count;
                    for (sf::Uint32 i = 0; i < game_objects_count; ++i)
                    {
                        bt::uuid game_object_id;
                        *packet_ref >> game_object_id;
                        bt::game_object_frame_restorer_packet restorer_packet{ packet_ref };
                        auto player = std::make_shared<bt::player_game_object>(game_object_id, texture_warehouse.load_pack(bt::textures_pack_id::tank), game->get_physics_body_factory());
                        player->initialize();
                        player->restore_from_frame(restorer_packet);
                        game->add_game_object(player);
                    }
                }
                break;
            }
            case command_id_server::update_game_frame:
            {
                if (player_state == player_game_state::game_session)
                {
                    sf::Uint32 game_objects_count;
                    *packet_ref >> game_objects_count;
                    for (sf::Uint32 i = 0; i < game_objects_count; ++i)
                    {
                        bt::uuid game_object_id;
                        *packet_ref >> game_object_id;

                        bt::game_object_frame_restorer_packet restorer_packet{ packet_ref };
                        game->get_game_scene()->get_game_object(game_object_id)->restore_from_frame(restorer_packet);
                    }
                }
                break;
            }
            case command_id_server::player_shoot:
            {
                if (player_state == player_game_state::game_session)
                {
                    bt::uuid player_id;
                    *packet_ref >> player_id;
                    bt::uuid bullet_id;
                    *packet_ref >> bullet_id;
                    bt::game_object_frame_restorer_packet restorer_packet{ packet_ref };
                    auto bullet = std::make_shared<bt::bullet_game_object>(
                        bullet_id,
                        game->get_physics_body_factory(),
                        texture_warehouse.load_texture(bt::texture_id::bullet)
                    );
                    bullet->initialize();
                    bullet->restore_from_frame(restorer_packet);
                    game->add_game_object(bullet);
                }
                break;
            }

            }
        }
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Escape))
            {
                window->close();
            }

            if (player_state == player_game_state::main_lobby)
            {
                //TODO:: don't allow to repeat it until server response
                if (event.type == sf::Event::KeyReleased)
                {
                    if (event.key.code == sf::Keyboard::C)
                    {
                        std::cout << "creating session... " << std::endl;
                        connection_service->send(client_command{ command_id_client::create_session });
                    }
                    else if (event.key.code == sf::Keyboard::J)
                    {
                        std::cout << "joining to session... id: " << 2 << std::endl;
                        player_state = player_game_state::session_lobby;
                        connection_service->send(join_session_command{ 2 });
                    }
                }
            }

            if (player_state == player_game_state::game_session)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                {
                    tank_move_action = player_action::move_forward;
                    connection_service->send(player_action_command{ player_action::move_forward });
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
                {
                    tank_move_action = player_action::move_backward;
                    connection_service->send(player_action_command{ player_action::move_backward });
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
                {
                    tank_rotate_action = player_action::turn_left;
                    connection_service->send(player_action_command{ player_action::turn_left });
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
                {
                    tank_rotate_action = player_action::turn_right;
                    connection_service->send(player_action_command{ player_action::turn_right });
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
                {
                    tower_rotate_action = player_action::turn_turret_left;
                    connection_service->send(player_action_command{ player_action::turn_turret_left });
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
                {
                    tower_rotate_action = player_action::turn_turret_right;
                    connection_service->send(player_action_command{ player_action::turn_turret_right });
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
                {
                    tank_fire = true;
                    connection_service->send(player_action_command{ player_action::turret_fire });
                }

                if (event.type == sf::Event::KeyReleased)
                {
                    if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::S)
                    {
                        tank_move_action = player_action::stop_move;
                        connection_service->send(player_action_command{ player_action::stop_move });
                    }

                    if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::D)
                    {
                        tank_rotate_action = player_action::stop_turn;
                        connection_service->send(player_action_command{ player_action::stop_turn });
                    }

                    if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right)
                    {
                        tower_rotate_action = player_action::stop_turn_turret;
                        connection_service->send(player_action_command{ player_action::stop_turn_turret });
                    }

                    if (event.key.code == sf::Keyboard::Space)
                    {
                        tank_fire = false;
                        connection_service->send(player_action_command{ player_action::turret_stop_fire });
                    }
                }
            }
        }

        if (tank_fire)
        {
            auto bullet_current_time = std::chrono::high_resolution_clock::now();

            if (std::chrono::duration_cast<std::chrono::milliseconds>(bullet_current_time - bullet_creation_time).count() >= 1000)
            {
                connection_service->send(player_action_command{ player_action::turret_fire });
                /*bullet_creation_time = bullet_current_time;
                const float bullet_rotation_rad = bt::deg_to_rad(tank_tower_sprite->getRotation() + tank_container->getRotation());
                const b2Vec2 bullet_pos{
                (tank_container->getPosition().x + 80 * std::sin(bullet_rotation_rad)) / pixels_per_meters,
                (tank_container->getPosition().y - 80 * std::cos(bullet_rotation_rad)) / pixels_per_meters
                };
                b2BodyDef bullet_body_def;
                bullet_body_def.type = b2_dynamicBody;
                bullet_body_def.position.Set(bullet_pos.x, bullet_pos.y);
                auto* bullet_body = game->get_physics_world()->CreateBody(&bullet_body_def);
                b2PolygonShape bullet_shape;
                bullet_shape.SetAsBox(
                    static_cast<float>(bullet_texture_render_data->get_size().x) / pixels_per_meters / 2,
                    static_cast<float>(bullet_texture_render_data->get_size().y) / pixels_per_meters / 2
                );
                auto* fixture = bullet_body->CreateFixture(&bullet_shape, 1.0f);
                bt::uuid bullet_id = bt::generate_uuid();
                auto* bullet_link = new bt::game_object_b2d_link{ bt::game_object_type::bullet, bullet_id };
                bullet_body->GetUserData().pointer = reinterpret_cast<uintptr_t>(bullet_link);

                b2Filter filter;
                filter.categoryBits = 0x0001;
                filter.maskBits = 0x8001;
                fixture->SetFilterData(filter);

                auto force = b2Vec2{
                    std::sin(bullet_rotation_rad) * 500.0f / pixels_per_meters,
                    -std::cos(bullet_rotation_rad) * 500.0f / pixels_per_meters
                };
                bullet_body->SetTransform(bullet_pos, bullet_rotation_rad);
                bullet_body->SetLinearVelocity(force);

                const auto bt = std::make_shared<bt::bullet>(bullet_id, 500.0f, bullet_texture_render_data, bullet_body);
                bt->initialize();
                bt->get_render_object()->setOrigin(
                    static_cast<float>(bullet_texture_render_data->get_size().x) / 2.0f,
                    static_cast<float>(bullet_texture_render_data->get_size().y) / 2.0f
                );
                game->get_game_scene()->add_game_object(bt);
                game->get_render_scene()->add_child(bt->get_render_object());*/
            }
        }

        game->update(delta_time);
    }

    return 0;
}
