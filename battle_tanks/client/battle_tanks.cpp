#include <iostream>
#include <chrono>
#include <memory>

#include "SFML/System/Clock.hpp"
#include "SFML/Graphics.hpp"
#include <SFML/Network.hpp>

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
#include "game/game_states/game_connection_states.hpp"


void set_text_color(sf::Text& text, const sf::Int32 current_hp)
{
    if (current_hp > 50)
    {
        text.setFillColor(sf::Color::Green);
    }
    else if (current_hp > 25)
    {
        text.setFillColor(sf::Color::Yellow);
    }
    else
    {
        text.setFillColor(sf::Color::Red);
    }
}

int main()
{
    auto window = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(
            bt::game_entity_consts::game_field_size.x,
            bt::game_entity_consts::game_field_size.y
        ),
        "Battle Tanks"
    );
    //window->setFramerateLimit(60);

    std::unordered_map<bt::uuid, std::shared_ptr<bt::player_game_object>> players {};

    sf::Clock clock;

    auto game = std::make_unique<bt::sfml_game>(
        window,
        bt::physics_consts::pixels_per_meters
    );

    auto texture_warehouse = std::make_shared<bt::texture_warehouse>();

    auto game_session = std::make_shared<bt::game_session>();

    bt::connection_state_manager connection_state_manager{
        game_session,
        texture_warehouse,
        game->get_physics_body_factory(),
        [&game, &players](const std::shared_ptr<bt::player_game_object>& player_game_object)
        {
            game->add_game_object(player_game_object);
            players.emplace(player_game_object->get_id(), player_game_object);
        },
        [&game](const std::shared_ptr<bt::game_object>& game_object)
        {
            game->add_game_object(game_object);
        },
        [&game, &players](const bt::uuid& game_object_id)
        {
            game->delete_game_object(game_object_id);
            if (players.contains(game_object_id))
            {
                players.erase(game_object_id);
            }
        },
        [&game](const bt::uuid game_object_id, const bt::game_object_frame_restorer_packet& restorer_packet)
        {
            game->get_game_scene()->get_game_object(game_object_id)->restore_from_frame(restorer_packet);
        }
    };
    connection_state_manager.start();

    sf::Font font;
    font.loadFromFile("game_data/fonts/wheaton capitals.otf");
    sf::Text player_hp;
    player_hp.setPosition(10, 10);
    player_hp.setFont(font);
    player_hp.setString("100");
    player_hp.setCharacterSize(24);
    player_hp.setFillColor(sf::Color::Green);
    player_hp.setStyle(sf::Text::Bold);

    sf::Text enemy_hp;
    enemy_hp.setFont(font);
    enemy_hp.setString("100");
    enemy_hp.setCharacterSize(24);
    enemy_hp.setFillColor(sf::Color::Green);
    enemy_hp.setStyle(sf::Text::Bold);

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

    texture_warehouse->pre_load_atlas(map_atlas);

    //TODO:: start move object immediately after key pressed. don't wait server to move 
    player_action tank_move_action{};
    player_action tank_rotate_action{};
    player_action tower_rotate_action{};
    bool tank_fire{ false };
    // ///////////////////////////

    auto forest_game_map = std::make_shared<bt::forest_map_game_object>(
        0,
        texture_warehouse->load_pack(bt::textures_pack_id::map_forest),
        bt::game_entity_consts::game_field_size,
        game->get_physics_body_factory()
    );
    forest_game_map->initialize();
    game->add_game_object(forest_game_map);

    auto rock_texture_render_data = std::make_shared<bt::texture_holder>("game_data/atlases/rock_1.png");
    forest_game_map->create_rock({ 200, 200 }, rock_texture_render_data);

    while (window->isOpen())
    {
        float delta_time = clock.restart().asSeconds();

        connection_state_manager.update(delta_time);

        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Escape))
            {
                window->close();
            }

            connection_state_manager.handle_event(event);
        }

        game->update(delta_time);

        for (const auto& player : players | std::views::values)
        {
            if (player->get_id() == game_session->get_player_id())
            {
                player_hp.setString(std::to_string(player->get_health()));
                set_text_color(player_hp, player->get_health());
            }
            else
            {
                enemy_hp.setString(std::to_string(player->get_health()));
                set_text_color(enemy_hp, player->get_health());
            }
        }

        enemy_hp.setPosition(static_cast<float>(bt::game_entity_consts::game_field_size.x) - enemy_hp.getLocalBounds().width - 15, 10);
        window->draw(player_hp);
        window->draw(enemy_hp);

        window->display();
    }

    return 0;
}
