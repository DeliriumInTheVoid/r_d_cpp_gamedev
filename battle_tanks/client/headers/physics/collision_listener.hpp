#pragma once

#include <functional>

#include "physics/game_object_b2d_link.hpp"

namespace bt
{
    class collision_listener final : public b2ContactListener
    {
    public:
        collision_listener() = default;

        virtual ~collision_listener() override
        {
            collision_handler_ = nullptr;
        }

    public:

        void add_collision_handler(const std::function<void(game_object_b2d_link game_object_a, game_object_b2d_link game_object_b)>& collision_handler)
        {
            collision_handler_ = collision_handler;
        }

        void BeginContact(b2Contact* contact) override
        {
            b2Fixture* fixture_a = contact->GetFixtureA();
            b2Fixture* fixture_b = contact->GetFixtureB();

            b2Body* body_a = fixture_a->GetBody();
            b2Body* body_b = fixture_b->GetBody();
            const b2BodyUserData user_data_a = body_a->GetUserData();
            const b2BodyUserData user_data_b = body_b->GetUserData();

            const game_object_b2d_link* link_a{ nullptr };
            const game_object_b2d_link* link_b{ nullptr };

            if (user_data_a.pointer != 0)
            {
                link_a = reinterpret_cast<game_object_b2d_link*>(user_data_a.pointer);
            }

            if (user_data_b.pointer != 0)
            {
                link_b = reinterpret_cast<game_object_b2d_link*>(user_data_b.pointer);
            }

            if (collision_handler_)
            {
                collision_handler_(
                    link_a != nullptr ? *link_a : game_object_b2d_link{ game_object_type::unknown, 0 },
                    link_b != nullptr ? *link_b : game_object_b2d_link{ game_object_type::unknown, 0 }
                );
            }
        }

    private:
        std::function<void(game_object_b2d_link game_object_a, game_object_b2d_link game_object_b)> collision_handler_{};
    };
}