#pragma once

#include <SFML/Config.hpp>

#include "box2d/b2_math.h"
#include "game/entity/game_object_type.hpp"
#include "game/game_objects/game_object_frame_restorer.hpp"
#include "network/commands.hpp"


namespace bt
{
    class game_object_entity
    {
    public:
        game_object_entity(const sf::Uint32 id, const bt::game_object_type type) : id_{ id }, type_ { type }
        {
        }

        virtual ~game_object_entity() = default;

    public:
        virtual void restore_frame(const bt::game_object_frame_restorer& restorer) = 0;
        virtual void update(const float delta_time) = 0;
        virtual bool is_out_of_edges(const b2Vec2& size) const = 0;

    public:
        sf::Uint32 get_id() const
        {
            return id_;
        }

        bt::game_object_type get_type() const
        {
            return type_;
        }

        [[nodiscard]]
        std::shared_ptr<game_object_frame> get_frame() const
        {
            auto object_frame = create_frame();
            fill_frame_data(object_frame);
            return object_frame;
        }

        void write_to_frame(const std::shared_ptr<game_object_frame>& object_frame) const
        {
            fill_frame_data(object_frame);
        }

    protected:
        [[nodiscard]]
        virtual std::shared_ptr<game_object_frame> create_frame() const = 0;

        virtual void fill_frame_data(const std::shared_ptr<game_object_frame>& object_frame) const
        {
            object_frame->game_object_id = id_;
            object_frame->game_object_type = type_;
        }

    protected:
        sf::Uint32 id_;
        bt::game_object_type type_;

    };
}
