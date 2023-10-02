#pragma once

#include <memory>

#include "game/game_objects/game_object.hpp"
#include "physics/physics_body_factory.hpp"
#include "renderer/textures.hpp"
#include "utils/uuid.hpp"


namespace bt
{
    //TODO:: create object poll for such objects
    class bullet_game_object : public bt::game_object
    {
    public:
        bullet_game_object(
            const bt::uuid id,
            const std::weak_ptr<physics_body_factory>& ph_body_factory,
            const std::shared_ptr<bt::texture_holder>& render_data
            );

        virtual ~bullet_game_object() override = default;

    public:
        virtual void restore_from_frame(const bt::game_object_frame_restorer& restorer) override;
        virtual void update(float delta_time) override;

    protected:
        virtual void create_render_object() override;
        virtual void create_game_object_entity() override;

    private:

        std::shared_ptr<bt::texture_holder> render_data_{ nullptr };
        const std::weak_ptr<physics_body_factory> ph_body_factory_;
    };
}
