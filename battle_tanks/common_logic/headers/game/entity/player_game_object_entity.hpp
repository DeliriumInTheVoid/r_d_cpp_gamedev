#pragma once

#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "SFML/Config.hpp"

#include "game/entity/phy_game_object_entity.hpp"
#include "game/entity/game_entity_constants.hpp"
#include "physics/physics_body_factory.hpp"
#include "game/player_action_controller.hpp"
#include "game/game_objects/game_object_frame_restorer.hpp"
#include "physics/game_object_b2d_link.hpp"


namespace bt
{
    class player_game_object_entity : public bt::phy_game_object_entity
    {
    public:
        player_game_object_entity(const sf::Uint32 id, const std::weak_ptr<physics_body_factory>& ph_body_factory)
            : bt::phy_game_object_entity(id, game_object_type::tank, ph_body_factory)
        {
        }

        virtual ~player_game_object_entity() override = default;

    public:
        virtual void create_phy_body() override
        {
            if (ph_body_factory_.expired())
            {
                return;
            }

            b2BodyDef tank_body_def;
            tank_body_def.type = b2_dynamicBody;
            phy_body_ = ph_body_factory_.lock()->create_body(tank_body_def);
            b2PolygonShape tank_shape;
            tank_shape.SetAsBox(
                bt::game_entity_consts::tank_size.x / bt::physics_consts::pixels_per_meters / 2,
                bt::game_entity_consts::tank_size.y / physics_consts::pixels_per_meters / 2
            );
            auto* tank_fixture = phy_body_->CreateFixture(&tank_shape, 1.0f);

            auto* bullet_link = new bt::game_object_b2d_link{ bt::game_object_type::tank, id_ };
            phy_body_->GetUserData().pointer = reinterpret_cast<uintptr_t>(bullet_link);

            b2Filter tank_filter;
            tank_filter.categoryBits = 0x0004;
            tank_filter.maskBits = 0x8007;//4-player, 2-rock, 1-bullet
            tank_fixture->SetFilterData(tank_filter);

            action_controller_ = std::make_shared<bt::player_action_controller>(
                phy_body_
                //move_speed_,
                //rotate_speed_
            );
        }

        virtual void update(const float dt) override
        {
            action_controller_->update(dt);
        }

        virtual void restore_frame(const bt::game_object_frame_restorer& restorer) override
        {
            player_game_object_frame frame{};
            restorer.restore_frame(frame);
            health_ = frame.health;

            phy_body_->SetTransform(
                b2Vec2(
                    frame.position.x,
                    frame.position.y
                ),
                frame.rotation
            );

            if (frame.velocity.x == 0.0f && frame.velocity.y == 0.0f)
            {
                phy_body_->SetLinearVelocity(
                    b2Vec2(
                        frame.velocity.x,
                        frame.velocity.y
                    )
                );
            }

            if (frame.velocity_angle == 0.0f)
            {
                phy_body_->SetAngularVelocity(frame.velocity_angle);
            }

            action_controller_->set_turret_rotation(frame.turret_rotation);
        }

    public:
        sf::Int32 get_health() const
        {
            return health_;
        }

        void take_damage(const sf::Int32 damage)
        {
	        health_ -= damage;
	        if (health_ < 0)
	        {
                health_ = 0;
	        }
        }

        [[nodiscard]]
        std::weak_ptr<bt::player_action_controller> get_action_controller() const
        {
            return action_controller_;
        }

        void create_physics_body(const b2Vec2 position, const float rotation = 0.0f)
        {
            create_phy_body();
            phy_body_->SetTransform(position, rotation);
        }

    protected:
        [[nodiscard]]
        virtual std::shared_ptr<game_object_frame> create_frame() const override
        {
            auto go_frame = std::make_unique<player_game_object_frame>();

            go_frame->health = health_;
            go_frame->player_score = 0;
            go_frame->turret_rotation = action_controller_->get_turret_rotation();

            return go_frame;
        }

        virtual void fill_frame_data(const std::shared_ptr<game_object_frame>& object_frame) const  override
        {
            bt::phy_game_object_entity::fill_frame_data(object_frame);
            auto* go_frame = dynamic_cast<player_game_object_frame*>(object_frame.get());
            go_frame->turret_rotation = action_controller_->get_turret_rotation();
        }

    private:
        sf::Int32 health_{ game_entity_consts::tank_hp };

        std::shared_ptr<bt::player_action_controller> action_controller_{ nullptr };

    };
}

