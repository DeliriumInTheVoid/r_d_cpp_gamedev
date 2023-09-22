#pragma once

#include <memory>

#include <Box2D/b2_body.h>

#include "utils/uuid.hpp"
#include "renderer/render_object.hpp"

namespace bt
{
    class game_object
    {
    public:
        explicit game_object(const bt::uuid id) : id_{ id }
        {
        }

        virtual ~game_object() = default;

    public:
        virtual void initialize() = 0;
        virtual void update(float delta_time) = 0;
        virtual void free() = 0;

    public:
        bt::uuid get_id() const
        {
            return id_;
        }

        const std::shared_ptr<bt::render_object>& get_render_object() const
        {
            return render_object_;
        }

        b2Body* get_physics_body() const
        {
            return physics_body_;
        }

    protected:
        std::shared_ptr<bt::render_object> render_object_{ nullptr };
        b2Body* physics_body_{ nullptr };
        bt::uuid id_;
    };

}
