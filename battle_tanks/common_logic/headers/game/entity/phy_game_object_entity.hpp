#pragma once

#include "SFML/Config.hpp"

#include "physics/physics_body_factory.hpp"
#include "game/entity/game_object_entity.hpp"


namespace bt
{
    class phy_game_object_entity : public bt::game_object_entity
    {
    public:
        phy_game_object_entity(const sf::Uint32 id, const bt::game_object_type type, const physics_body_factory& ph_body_factory)
            : bt::game_object_entity(id, type), ph_body_factory_{ ph_body_factory }
        {
        }

        virtual ~phy_game_object_entity() override
        {
            if (phy_body_ != nullptr)
            {
                ph_body_factory_.destroy_body(phy_body_);
                phy_body_ = nullptr;
            }
        }

    public:
        virtual void create_phy_body() = 0;

    protected:
        virtual void set_frame_data(const std::shared_ptr<game_object_frame>& object_frame) const  override
        {
            bt::game_object_entity::set_frame_data(object_frame);
            object_frame->position = { phy_body_->GetPosition().x, phy_body_->GetPosition().y };
            object_frame->rotation = phy_body_->GetAngle();
            object_frame->velocity = { phy_body_->GetLinearVelocity().x, phy_body_->GetLinearVelocity().y };
            object_frame->velocity_angle = phy_body_->GetAngularVelocity();
        }

    protected:
        b2Body* phy_body_{ nullptr };
        const physics_body_factory& ph_body_factory_;
    };
}