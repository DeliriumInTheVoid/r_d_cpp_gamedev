#include "game/game_objects//bullet_game_object.hpp"

#include "game/entity/bullet_game_object_entity.hpp"
#include "game/entity/game_entity_constants.hpp"
#include "renderer/sprite.hpp"

#include "utils/math.hpp"


bt::bullet_game_object::bullet_game_object(
    const bt::uuid id,
    const std::weak_ptr<physics_body_factory>& ph_body_factory,
    const std::shared_ptr<bt::texture_holder>& render_data)
    : bt::game_object(id), render_data_{ render_data }, ph_body_factory_(ph_body_factory)
{
}

void bt::bullet_game_object::restore_from_frame(const bt::game_object_frame_restorer& restorer)
{
    game_object_entity_->restore_frame(restorer);
}

void bt::bullet_game_object::create_render_object()
{
    render_object_ = std::make_shared<bt::sprite>(render_data_);
    render_object_->setOrigin(
        game_entity_consts::bullet_size.x / 2.0f,
        game_entity_consts::bullet_size.y / 2.0f
    );
}

void bt::bullet_game_object::create_game_object_entity()
{
    auto bullet_entity = std::make_unique<bt::bullet_game_object_entity>(id_, ph_body_factory_/*, velocity_, rotation_rad_, tank_position_*/);
    bullet_entity->create_phy_body();
    game_object_entity_ = std::move(bullet_entity);
}

void bt::bullet_game_object::update(float delta_time)
{
    const auto frame = std::make_shared<player_game_object_frame>();
    game_object_entity_->write_to_frame(frame);
    const auto& ph_body_pos = frame->position;
    const float ph_body_rot = frame->rotation;

    render_object_->setPosition({ ph_body_pos.x * physics_consts::pixels_per_meters, ph_body_pos.y * physics_consts::pixels_per_meters });
    render_object_->setRotation(std::fmod(bt::rad_to_deg(ph_body_rot), 360.0f));
}
