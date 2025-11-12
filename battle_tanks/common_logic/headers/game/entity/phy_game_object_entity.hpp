#pragma once

#include "SFML/Config.hpp"

#include "physics/physics_body_factory.hpp"
#include "game/entity/game_object_entity.hpp"


namespace bt
{
    class phy_game_object_entity : public bt::game_object_entity
    {
    public:
        phy_game_object_entity(const sf::Uint32 id, const bt::game_object_type type, const std::weak_ptr<physics_body_factory>& ph_body_factory)
            : bt::game_object_entity(id, type), ph_body_factory_{ ph_body_factory }
        {
        }

        virtual ~phy_game_object_entity() override
        {
            if (phy_body_ != nullptr && !ph_body_factory_.expired())
            {
                ph_body_factory_.lock()->destroy_body(phy_body_);
                phy_body_ = nullptr;
            }
        }

    public:
        virtual void create_phy_body() = 0;

        virtual bool is_out_of_edges(const b2Vec2& size) const override
        {
            if (phy_body_ == nullptr)
            {
                return false;
            }
            const auto pos = phy_body_->GetPosition();
            return pos.x < 0.0f || pos.x > size.x || pos.y < 0.0f || pos.y > size.y;
        }

    protected:
        virtual void fill_frame_data(const std::shared_ptr<game_object_frame>& object_frame) const  override
        {
            bt::game_object_entity::fill_frame_data(object_frame);
            object_frame->position = { phy_body_->GetPosition().x, phy_body_->GetPosition().y };
            object_frame->rotation = phy_body_->GetAngle();
            object_frame->velocity = { phy_body_->GetLinearVelocity().x, phy_body_->GetLinearVelocity().y };
            object_frame->velocity_angle = phy_body_->GetAngularVelocity();
        }

    protected:
        b2Body* phy_body_{ nullptr };
        std::weak_ptr<physics_body_factory> ph_body_factory_;
    };
}
