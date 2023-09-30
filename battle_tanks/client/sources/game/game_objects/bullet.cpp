#include <box2d/b2_math.h>

#include "game/game_objects//bullet.hpp"
#include "renderer/sprite.hpp"

#include "utils/math.hpp"

bt::bullet::bullet(const bt::uuid id, float velocity, const std::shared_ptr<bt::texture_holder>& render_data, b2Body* body) :
    bt::game_object(id), velocity_{ velocity }, render_data_{ render_data }
{

}

//void bt::bullet::initialize()
//{
//    render_object_ = std::make_shared<bt::sprite>(render_data_);
//}

void bt::bullet::update(float delta_time)
{
    //if (render_object_ == nullptr || velocity_ == 0.0f)
    //{
    //    return;
    //}

    //auto& ph_body_pos = physics_body_->GetPosition();
    //const float ph_body_rot = physics_body_->GetAngle();

    ////TODO 10.0f move to constants
    //render_object_->setPosition({ ph_body_pos.x * 10.0f, ph_body_pos.y * 10.0f });
    //render_object_->setRotation(rad_to_deg_360(ph_body_rot));
}

void bt::bullet::free()
{
    /*if (render_object_ != nullptr)
    {
        const auto render_object = render_object_;
        render_object_ = nullptr;
        render_object->free();
    }*/
}
