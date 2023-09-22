#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "unordered_set"
#include "dev/debug_draw.hpp"

#include "physics/collision_listener.hpp"


namespace bt
{
    class sfml_game
    {
    public:
        sfml_game(sf::Vector2u field_size, const std::shared_ptr<sf::RenderWindow>& render_target, const float pixels_per_meters)
            : window_{ render_target }, pixels_per_meters_{ pixels_per_meters }
        {
            game_world_ = std::make_unique<bt::game_scene>();
            render_scene_ = std::make_unique<bt::render_scene>(render_target);
            physics_world_ = std::make_unique<b2World>(b2Vec2{ 0.0f, 0.0f });
            physics_world_->SetAllowSleeping(true);
            //physics_world.SetContinuousPhysics(true);
            contact_listener_ = new bt::collision_listener();
            physics_world_->SetContactListener(contact_listener_);
            debug_draw_ = new SFMLDebugDraw(render_target, pixels_per_meters_);
            debug_draw_->AppendFlags(b2Draw::e_shapeBit);
            physics_world_->SetDebugDraw(debug_draw_);

            contact_listener_->add_collision_handler([this](bt::game_object_b2d_link game_object_a, bt::game_object_b2d_link game_object_b)
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
        }

        ~sfml_game()
        {
            delete contact_listener_;
            delete debug_draw_;
        }

    public:
        const std::unique_ptr<bt::game_scene>& get_game_scene() const
        {
            return game_world_;
        }

        const std::unique_ptr<bt::render_scene>& get_render_scene() const
        {
            return render_scene_;
        }

        const std::unique_ptr<b2World>& get_physics_world() const
        {
            return physics_world_;
        }

        void update(const float delta_time)
        {
            window_->clear(sf::Color::Black);

            delete_game_objects();

            physics_world_->Step(delta_time, 8, 3);
            game_world_->update(delta_time);
            render_scene_->draw_scene();
            physics_world_->DebugDraw();

            window_->display();
        }

    private:
        void delete_game_objects()
        {
            for (const auto& game_object_id : objects_to_delete_)
            {
                auto game_object = game_world_->get_game_object(game_object_id);
                physics_world_->DestroyBody(game_object->get_physics_body());
                game_object->free();
                game_world_->remove_game_object(game_object);
            }
            objects_to_delete_.clear();
        }

    private:
        float pixels_per_meters_{};

        std::shared_ptr<sf::RenderWindow> window_{ nullptr };
        std::unique_ptr<bt::game_scene> game_world_{ nullptr };
        std::unique_ptr<bt::render_scene> render_scene_{ nullptr };
        std::unique_ptr<b2World> physics_world_{ nullptr };
        SFMLDebugDraw* debug_draw_{ nullptr };
        bt::collision_listener* contact_listener_{ nullptr };

        std::unordered_set<bt::uuid> objects_to_delete_{};
    };

}
