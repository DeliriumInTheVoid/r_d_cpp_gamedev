#include <cmath>
#include <numbers>
#include <chrono>
#include <functional>

#include "SFML/System/Clock.hpp"
#include "SFML/Graphics.hpp"
#include <box2d/box2d.h>

#include "utils/uuid.hpp"
#include "utils/math.hpp"
#include "renderer/renderer.hpp"
#include "game/entity/game_entities.hpp"
#include "game/sfml_game.hpp"
#include "physics/collision_listener.hpp"

enum class move_action
{
    stop = 0,
    forward,
    backward,
};

enum rotate_action
{
    stop = 0,
    left,
    right,
};

int main()
{
    constexpr float pixels_per_meters{ 10.0f }; // 1 meter = 10 pixels
    const sf::Vector2u game_field_size{ 800, 600 };

    auto window = std::make_shared<sf::RenderWindow>(sf::VideoMode(game_field_size.x, game_field_size.y), "Battle Tanks");
    //window->setFramerateLimit(20);

    sf::Clock clock;

    std::unique_ptr<bt::sfml_game> game = std::make_unique<bt::sfml_game>(game_field_size, window, pixels_per_meters);

    // <TEXTURES>
    auto tank_texture_render_data = std::make_shared<bt::texture_render_data>("game_data/atlases/tank.png");
    auto tower_texture_render_data = std::make_shared<bt::texture_render_data>("game_data/atlases/tower.png");
    auto bullet_texture_render_data = std::make_shared<bt::texture_render_data>("game_data/atlases/bullet_1.png");
    // </TEXTURES>

    // <TANK>
    move_action tank_move_action{};
    rotate_action tank_rotate_action{};
    rotate_action tower_rotate_action{};
    bool tank_fire{ false };

    sf::Vector2u tank_texture_size = tank_texture_render_data->get_size();

    const auto tank_container = std::make_shared<bt::container>();
    tank_container->setPosition(100.0, 100.0);
    tank_container->setOrigin(static_cast<float>(tank_texture_size.x) / 2.0f, static_cast<float>(tank_texture_size.y) / 2.0f);
    game->get_render_scene()->add_child(tank_container);

    const auto tank_base_sprite = std::make_shared<bt::sprite>(tank_texture_render_data);
    tank_container->add_child(tank_base_sprite);

    const auto tank_tower_sprite = std::make_shared<bt::sprite>(tower_texture_render_data);
    tank_tower_sprite->setPosition(16.5f, 27.0f);
    tank_tower_sprite->setOrigin(10.0f, 67.0f);
    tank_container->add_child(tank_tower_sprite);

    b2BodyDef tank_body_def;
    tank_body_def.type = b2_dynamicBody;
    tank_body_def.position.Set(100.0f / pixels_per_meters, 100.0f / pixels_per_meters);
    auto tank_body = game->get_physics_world()->CreateBody(&tank_body_def);
    b2PolygonShape tank_shape;
    tank_shape.SetAsBox(
        static_cast<float>(tank_texture_size.x) / pixels_per_meters / 2,
        static_cast<float>(tank_texture_size.y) / pixels_per_meters / 2
    );
    tank_body->CreateFixture(&tank_shape, 1.0f);
    // </TANK>

    // <EDGES>
    b2Vec2 bottom_right_point = b2Vec2{
            static_cast<float>(game_field_size.x) / pixels_per_meters,
            static_cast<float>(game_field_size.y) / pixels_per_meters
    };
    b2BodyDef edge_body_def;
    edge_body_def.type = b2_staticBody;
    auto edges_body = game->get_physics_world()->CreateBody(&edge_body_def);

    b2EdgeShape top_edge_shape;
    top_edge_shape.SetTwoSided(b2Vec2(0.0f, 0.0f), b2Vec2(bottom_right_point.x, 0.0f));
    edges_body->CreateFixture(&top_edge_shape, 1.0f);

    b2EdgeShape bottom_edge_shape;
    bottom_edge_shape.SetTwoSided(b2Vec2(0.0f, bottom_right_point.y), bottom_right_point);
    edges_body->CreateFixture(&bottom_edge_shape, 1.0f);

    b2EdgeShape left_edge_shape;
    left_edge_shape.SetTwoSided(b2Vec2(0.0f, 0.0f), b2Vec2(0.0f, bottom_right_point.y));
    edges_body->CreateFixture(&left_edge_shape, 1.0f);

    b2EdgeShape right_edge_shape;
    right_edge_shape.SetTwoSided(b2Vec2(bottom_right_point.x, 0.0f), bottom_right_point);
    edges_body->CreateFixture(&right_edge_shape, 1.0f);
    // </EDGES>

    auto bullet_creation_time = std::chrono::high_resolution_clock::now();
    while (window->isOpen())
    {
        float delta_time = clock.restart().asSeconds();

        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Escape))
            {
                window->close();
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            {
                tank_move_action = move_action::forward;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
            {
                tank_move_action = move_action::backward;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            {
                tank_rotate_action = left;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            {
                tank_rotate_action = right;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            {
                tower_rotate_action = left;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            {
                tower_rotate_action = right;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
            {
                tank_fire = true;
            }

            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::S)
                {
                    tank_move_action = move_action::stop;
                }

                if (event.key.code == sf::Keyboard::A || event.key.code == sf::Keyboard::D)
                {
                    tank_rotate_action = stop;
                }

                if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right)
                {
                    tower_rotate_action = stop;
                }

                if (event.key.code == sf::Keyboard::Space)
                {
                    tank_fire = false;
                }

                if (event.key.code == sf::Keyboard::Q)
                {
                    tank_body->SetTransform({ 100.0f / pixels_per_meters, 100.0f / pixels_per_meters }, tank_body->GetAngle());
                }
            }
        }

        if (tank_fire)
        {
            auto bullet_current_time = std::chrono::high_resolution_clock::now();

            if (std::chrono::duration_cast<std::chrono::milliseconds>(bullet_current_time - bullet_creation_time).count() >= 1000)
            {
                bullet_creation_time = bullet_current_time;
                const float bullet_rotation_rad = bt::deg_to_rad(tank_tower_sprite->getRotation() + tank_container->getRotation());
                const b2Vec2 bullet_pos{
                (tank_container->getPosition().x + 80 * std::sin(bullet_rotation_rad)) / pixels_per_meters,
                (tank_container->getPosition().y - 80 * std::cos(bullet_rotation_rad)) / pixels_per_meters
                };
                b2BodyDef bullet_body_def;
                bullet_body_def.type = b2_dynamicBody;
                bullet_body_def.position.Set(bullet_pos.x, bullet_pos.y);
                auto bullet_body = game->get_physics_world()->CreateBody(&bullet_body_def);
                b2PolygonShape bullet_shape;
                bullet_shape.SetAsBox(
                    static_cast<float>(bullet_texture_render_data->get_size().x) / pixels_per_meters / 2,
                    static_cast<float>(bullet_texture_render_data->get_size().y) / pixels_per_meters / 2
                );
                bullet_body->CreateFixture(&bullet_shape, 1.0f);
                bt::uuid bullet_id = bt::generate_uuid();
                auto* bullet_link = new bt::game_object_b2d_link{ bt::game_object_type::bullet, bullet_id };
                bullet_body->GetUserData().pointer = reinterpret_cast<uintptr_t>(bullet_link);

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
                game->get_render_scene()->add_child(bt->get_render_object());
            }
        }

        const auto rad_rotation = tank_body->GetAngle();
        auto tower_rotation = tank_tower_sprite->getRotation();

        constexpr float move_speed = 100.0;
        if (tank_move_action == move_action::forward)
        {
            auto force = b2Vec2{
                    std::sin(rad_rotation) * move_speed / pixels_per_meters,
                    -std::cos(rad_rotation) * move_speed / pixels_per_meters
            };

            tank_body->SetLinearVelocity(force);
        }
        else if (tank_move_action == move_action::backward)
        {
            auto force = b2Vec2{
                    -std::sin(rad_rotation) * move_speed / pixels_per_meters,
                    std::cos(rad_rotation) * move_speed / pixels_per_meters
            };

            tank_body->SetLinearVelocity(force);
        }
        else
        {
            tank_body->SetLinearVelocity({ 0.0f, 0.0f });
        }

        constexpr float rotate_speed = 90.0f;
        constexpr float rotate_speed_rad = static_cast<float>(std::numbers::pi) / 2.0f;
        if (tank_rotate_action == left)
        {
            tank_body->SetAngularVelocity(-rotate_speed_rad);
        }
        else if (tank_rotate_action == right)
        {
            tank_body->SetAngularVelocity(rotate_speed_rad);
        }
        else
        {
            tank_body->SetAngularVelocity(0.0f);
        }

        if (tower_rotate_action == left)
        {
            tower_rotation -= rotate_speed * delta_time;
        }
        else if (tower_rotate_action == right)
        {
            tower_rotation += rotate_speed * delta_time;
        }

        auto& ph_body_pos = tank_body->GetPosition();
        float ph_body_rot = tank_body->GetAngle();

        tank_container->setPosition({ph_body_pos.x * pixels_per_meters, ph_body_pos.y * pixels_per_meters});
        tank_container->setRotation(std::fmod(bt::rad_to_deg(ph_body_rot), 360.0f));
        tank_tower_sprite->setRotation(tower_rotation);

        game->update(delta_time);
    }

    game->get_physics_world()->DestroyBody(edges_body);
    game->get_physics_world()->DestroyBody(tank_body);

    tank_body = nullptr;

    return 0;
}
