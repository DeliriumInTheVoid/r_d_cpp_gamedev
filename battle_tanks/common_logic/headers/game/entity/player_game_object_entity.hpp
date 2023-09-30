#pragma once

#include "box2d/b2_fixture.h"
#include "box2d/b2_polygon_shape.h"
#include "SFML/Config.hpp"

#include "game/entity/phy_game_object_entity.hpp"
#include "game/entity/game_entity_constants.hpp"
#include "physics/physics_body_factory.hpp"
#include "game/player_action_controller.hpp"
#include "game/game_objects/game_object_frame_restorer.hpp"


namespace bt
{
    class game_object_frame_restorer;

    class player_game_object_entity : public bt::phy_game_object_entity
    {
    public:
        player_game_object_entity(const sf::Uint32 id, const physics_body_factory& ph_body_factory)
            : bt::phy_game_object_entity(id, game_object_type::tank, ph_body_factory)
        {
        }

        virtual ~player_game_object_entity() override = default;

    public:
        virtual void create_phy_body() override
        {
            b2BodyDef tank_body_def;
            tank_body_def.type = b2_dynamicBody;
            //tank_body_def.position.Set(
            //    100.0f / bt::physics_consts::pixels_per_meters,
            //    100.0f / bt::physics_consts::pixels_per_meters
            //);
            phy_body_ = ph_body_factory_.create_body(tank_body_def);
            b2PolygonShape tank_shape;
            tank_shape.SetAsBox(
                bt::game_entity_consts::tank_size.x / bt::physics_consts::pixels_per_meters / 2,
                bt::game_entity_consts::tank_size.y / physics_consts::pixels_per_meters / 2
            );
            auto* tank_fixture = phy_body_->CreateFixture(&tank_shape, 1.0f);

            b2Filter tank_filter;
            tank_filter.categoryBits = 0x0004;
            tank_filter.maskBits = 0x8002;
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

    public:

        [[nodiscard]]
        std::weak_ptr<bt::player_action_controller> get_action_controller() const
        {
            return action_controller_;
        }

        virtual void restore_frame(const bt::game_object_frame_restorer& restorer) const override
        {
            player_game_object_frame frame{};
            restorer.restore_frame(frame);

            phy_body_->SetTransform(
                b2Vec2(
                    frame.position.x,
                    frame.position.y
                ),
                frame.rotation
            );
            //phy_body_->SetLinearVelocity(
            //    b2Vec2(
            //        frame.velocity.x,
            //        frame.velocity.y
            //    )
            //);
            //phy_body_->SetAngularVelocity(frame.velocity_angle);
            phy_body_->SetAwake(true);

            action_controller_->set_turret_rotation(frame.turret_rotation);
        }

    protected:
        [[nodiscard]]
        virtual std::shared_ptr<game_object_frame> create_frame() const override
        {
            auto go_frame = std::make_unique<player_game_object_frame>();

            go_frame->health = 100;
            go_frame->player_score = 0;
            go_frame->turret_rotation = action_controller_->get_turret_rotation();

            return go_frame;
        }

        virtual void set_frame_data(const std::shared_ptr<game_object_frame>& object_frame) const  override
        {
            bt::phy_game_object_entity::set_frame_data(object_frame);
            auto* go_frame = dynamic_cast<player_game_object_frame*>(object_frame.get());
            go_frame->turret_rotation = action_controller_->get_turret_rotation();
        }

    private:
        //TODO:: move to config
        float move_speed_ = 100.0f;
        float rotate_speed_ = 90.0f;

        std::shared_ptr<bt::player_action_controller> action_controller_{ nullptr };

    };
}

