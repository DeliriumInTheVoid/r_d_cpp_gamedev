#pragma once

#include <memory>

#include "game/entity/game_object_entity.hpp"
#include "game/game_objects/game_object_frame_restorer.hpp"
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
        virtual void restore_from_frame(const bt::game_object_frame_restorer& restorer) = 0;
        virtual void update(float delta_time) = 0;

    public:
        bt::uuid get_id() const
        {
            return id_;
        }

        const std::shared_ptr<bt::render_object>& get_render_object() const
        {
            return render_object_;
        }

        void initialize()
        {
            create_game_object_entity();
            create_render_object();
        }

    protected:
        virtual void create_render_object() = 0;
        virtual void create_game_object_entity() = 0;

    protected:
        bt::uuid id_;
        std::shared_ptr<bt::render_object> render_object_{ nullptr };
        std::unique_ptr<game_object_entity> game_object_entity_{ nullptr };
    };

}
