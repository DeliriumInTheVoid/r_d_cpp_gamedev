#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "unordered_set"
#include "dev/debug_draw.hpp"

#include "physics/collision_listener.hpp"
#include "game_objects/game_scene.hpp"
#include "physics/physics_body_factory.hpp"
#include "renderer/render_scene.hpp"


namespace bt
{
    class sfml_game
    {
    public:
        sfml_game(sf::Vector2u field_size, const std::shared_ptr<sf::RenderWindow>& render_target, const float pixels_per_meters)
            : pixels_per_meters_{ pixels_per_meters }, window_{ render_target }
        {
            game_world_ = std::make_unique<bt::game_scene>();
            render_scene_ = std::make_unique<bt::render_scene>(render_target);
            //physics_world_ = std::make_unique<b2World>(b2Vec2{ 0.0f, 0.0f });
            physics_world_.SetAllowSleeping(true);
            //physics_world.SetContinuousPhysics(true);
            contact_listener_ = std::make_unique<bt::collision_listener>();
            physics_world_.SetContactListener(contact_listener_.get());
            debug_draw_ = new SFMLDebugDraw(render_target, pixels_per_meters_);
            debug_draw_->AppendFlags(b2Draw::e_shapeBit);
            physics_world_.SetDebugDraw(debug_draw_);

            contact_listener_->add_collision_handler([this](bt::game_object_b2d_link game_object_a, bt::game_object_b2d_link game_object_b)
                {
                    if (game_object_a.type == bt::game_object_type::bullet)
                    {
                        //objects_to_delete_.insert(game_object_a.id);
                    }
                    else if (game_object_b.type == bt::game_object_type::bullet)
                    {
                        //objects_to_delete_.insert(game_object_b.id);
                    }
                });

            physics_body_factory_ = std::make_shared<physics_body_factory>(&physics_world_);
        }

        ~sfml_game()
        {
            delete debug_draw_;
        }

    public:
        void add_game_object(const std::shared_ptr<bt::game_object>& game_object)
        {
            game_world_->add_game_object(game_object);
            render_scene_->add_child(game_object->get_render_object());
        }

        void delete_game_object(const bt::uuid game_object_id)
        {
            objects_to_delete_.insert(game_object_id);
        }

        const std::unique_ptr<bt::game_scene>& get_game_scene() const
        {
            return game_world_;
        }

        const std::unique_ptr<bt::render_scene>& get_render_scene() const
        {
            return render_scene_;
        }

        std::weak_ptr<bt::physics_body_factory> get_physics_body_factory() const
        {
            return physics_body_factory_;
        }

        float get_pixels_per_meters() const
        {
            return pixels_per_meters_;
        }

        void update(const float delta_time)
        {
            window_->clear(sf::Color::Black);

            delete_game_objects();

            physics_world_.Step(delta_time, 8, 3);
            game_world_->update(delta_time);
            render_scene_->draw_scene();
            physics_world_.DebugDraw();

            window_->display();
        }

    private:
        void delete_game_objects()
        {
            for (const auto& game_object_id : objects_to_delete_)
            {
                auto game_object = game_world_->get_game_object(game_object_id);
                render_scene_->remove_child(game_object->get_render_object());
                game_world_->remove_game_object(game_object);
            }
            objects_to_delete_.clear();
        }

    private:
        float pixels_per_meters_{};

        std::shared_ptr<sf::RenderWindow> window_{ nullptr };
        std::unique_ptr<bt::game_scene> game_world_{ nullptr };
        std::unique_ptr<bt::render_scene> render_scene_{ nullptr };
        b2World physics_world_{ b2Vec2{ 0.0f, 0.0f } };
        std::shared_ptr<bt::physics_body_factory> physics_body_factory_{ nullptr};
        SFMLDebugDraw* debug_draw_{ nullptr };
        std::unique_ptr<bt::collision_listener> contact_listener_{ nullptr };

        std::unordered_set<bt::uuid> objects_to_delete_{};
    };

}
