#pragma once

#include <box2d/b2_world.h>


namespace bt
{
    class physics_body_factory
    {
    public:
        physics_body_factory(b2World* world) : world_{ world }
        {
        }

        ~physics_body_factory() = default;

    public:

        b2Body* create_body(const b2BodyDef& body_def) const
        {
            return world_->CreateBody(&body_def);
        }

        void destroy_body(b2Body* body) const
        {
            if (body == nullptr)
            {
                return;
            }
            world_->DestroyBody(body);
        }

    private:
        b2World* world_;
    };
}