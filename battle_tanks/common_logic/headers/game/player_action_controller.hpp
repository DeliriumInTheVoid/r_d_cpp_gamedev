#pragma once

#include <numbers>

#include "network/player_actions.hpp"

namespace bt
{
    class player_action_controller
    {
    public:
        explicit player_action_controller(b2Body* physics_body) : phy_body_(physics_body)
        {
        }

        ~player_action_controller() = default;

    public:
        void set_move_action(const player_action action)
        {
            move_action_ = action;

            const auto rad_rotation = phy_body_->GetAngle();
            constexpr float move_speed = 100.0;
            if (move_action_ == player_action::move_forward)
            {
                const auto force = b2Vec2{
                        std::sin(rad_rotation) * move_speed / physics_consts::pixels_per_meters,
                        -std::cos(rad_rotation) * move_speed / physics_consts::pixels_per_meters
                };

                phy_body_->SetLinearVelocity(force);
            }
            else if (move_action_ == player_action::move_backward)
            {
                const auto force = b2Vec2{
                        -std::sin(rad_rotation) * move_speed / physics_consts::pixels_per_meters,
                        std::cos(rad_rotation) * move_speed / physics_consts::pixels_per_meters
                };

                phy_body_->SetLinearVelocity(force);
            }
            else
            {
                phy_body_->SetLinearVelocity({ 0.0f, 0.0f });
            }
        }

        void set_rotate_action(const player_action action)
        {
            rotate_action_ = action;

            constexpr float rotate_speed_rad = static_cast<float>(std::numbers::pi) / 2.0f;
            if (rotate_action_ == player_action::turn_left)
            {
                phy_body_->SetAngularVelocity(-rotate_speed_rad);
            }
            else if (rotate_action_ == player_action::turn_right)
            {
                phy_body_->SetAngularVelocity(rotate_speed_rad);
            }
            else
            {
                phy_body_->SetAngularVelocity(0.0f);
            }
        }

        void set_turret_rotate_action(const player_action action)
        {
            //TODO:: add independent from game session loop delta_time
            turret_rotate_action_ = action;
        }

        void set_turret_fire_action(const player_action action)
        {
            turret_fire_ = action;
        }

        float get_turret_rotation() const
        {
            return turret_rotation_;
        }

        void set_turret_rotation(const float rotation)
        {
            turret_rotation_ = rotation;
        }

        bool turret_fire()
        {
            
        }

        void update(const float delta_time)
        {
            constexpr float rotate_speed = 90.0f;
            if (turret_rotate_action_ == player_action::turn_turret_left)
            {
                turret_rotation_ -= rotate_speed * delta_time;
            }
            else if (turret_rotate_action_ == player_action::turn_turret_right)
            {
                turret_rotation_ += rotate_speed * delta_time;
            }
        }


    private:
        player_action move_action_{player_action::stop_move};
        player_action rotate_action_{player_action::stop_turn};
        player_action turret_rotate_action_{player_action::stop_turn_turret};
        player_action turret_fire_{player_action::turret_stop_fire};

        float turret_rotation_{0.0f};

        b2Body* phy_body_{ nullptr };
    };
}
