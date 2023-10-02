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
    class bullet_game_object_entity : public bt::phy_game_object_entity
    {
        public:
            bullet_game_object_entity(const sf::Uint32 id, const std::weak_ptr<physics_body_factory>& ph_body_factory)
                : bt::phy_game_object_entity(id, game_object_type::bullet, ph_body_factory)
            {
            }

            virtual ~bullet_game_object_entity() override = default;

        public:
            virtual void create_phy_body() override
            {
                if (ph_body_factory_.expired())
                {
                    return;
                }
                b2BodyDef bullet_body_def;
                bullet_body_def.type = b2_dynamicBody;
                phy_body_ = ph_body_factory_.lock()->create_body(bullet_body_def);
                b2PolygonShape bullet_shape;
                bullet_shape.SetAsBox(
                    game_entity_consts::bullet_size.x / physics_consts::pixels_per_meters / 2,
                    game_entity_consts::bullet_size.y / physics_consts::pixels_per_meters / 2
                );
                auto* fixture = phy_body_->CreateFixture(&bullet_shape, 1.0f);

                auto* bullet_link = new bt::game_object_b2d_link{ bt::game_object_type::bullet, id_ };
                phy_body_->GetUserData().pointer = reinterpret_cast<uintptr_t>(bullet_link);

                b2Filter filter;
                filter.categoryBits = 0x0001;
                filter.maskBits = 0x8001;
                fixture->SetFilterData(filter);
            }

            virtual void update(const float dt) override
            {
            }

            virtual void restore_frame(const bt::game_object_frame_restorer& restorer) const override
            {
                game_object_frame frame{};
                restorer.restore_frame(frame);

                phy_body_->SetTransform(
                    b2Vec2(
                        frame.position.x,
                        frame.position.y
                    ),
                    frame.rotation
                );
                phy_body_->SetLinearVelocity(
                    b2Vec2(
                        frame.velocity.x,
                        frame.velocity.y
                    )
                );
                //phy_body_->SetAngularVelocity(frame.velocity_angle);
                phy_body_->SetAwake(true);
            }
    public:
        void create_physics_body(const b2Vec2& player_position, const float rotation_rad, const float velocity)
        {
            create_phy_body();
            const b2Vec2 bullet_pos{
               (player_position.x + game_entity_consts::turret_length * std::sin(rotation_rad)),
               (player_position.y - game_entity_consts::turret_length * std::cos(rotation_rad))
            };

            const auto force = b2Vec2{
                   std::sin(rotation_rad) * velocity,
                   -std::cos(rotation_rad) * velocity
            };
            phy_body_->SetTransform(bullet_pos, rotation_rad);
            phy_body_->SetLinearVelocity(force);
        }

    protected:
        [[nodiscard]]
        virtual std::shared_ptr<game_object_frame> create_frame() const override
        {
            auto go_frame = std::make_unique<game_object_frame>();
            return go_frame;
        }

    };
}
