#pragma once

#include <memory>

#include <SFML/System/Vector2.hpp>

#include "game/entity/player_game_object_entity.hpp"
#include "game/game_objects/game_object.hpp"
#include "renderer/container.hpp"
#include "renderer/sprite.hpp"
#include "renderer/textures.hpp"
#include "utils/uuid.hpp"


namespace bt
{
    class player_game_object : public bt::game_object
    {
    public:
        player_game_object(
            const bt::uuid id,
            const std::shared_ptr<bt::textures_pack>& textures_pack,
            const std::weak_ptr<physics_body_factory>& ph_body_factory)
            : game_object(id), textures_pack_{ textures_pack }, ph_body_factory_{ ph_body_factory }
        {
        }

        virtual ~player_game_object() override = default;

    public:
        virtual void restore_from_frame(const bt::game_object_frame_restorer& restorer) override
        {
            game_object_entity_->restore_frame(restorer);
        }

        virtual void update(float delta_time) override
        {
            const auto frame = std::make_shared<player_game_object_frame>();
            game_object_entity_->write_to_frame(frame);
            const auto& ph_body_pos = frame->position;
            const float ph_body_rot = frame->rotation;

            render_object_->setPosition({ph_body_pos.x * physics_consts::pixels_per_meters, ph_body_pos.y * physics_consts::pixels_per_meters});
            render_object_->setRotation(std::fmod(bt::rad_to_deg(ph_body_rot), 360.0f));

            if (!turret_render_object_.expired())
            {
                turret_render_object_.lock()->setRotation(frame->turret_rotation);
            }
        }

    public:
        sf::Int32 get_health() const
        {
            return dynamic_cast<player_game_object_entity*>(game_object_entity_.get())->get_health();
        }

    protected:
        virtual void create_render_object() override
        {
            auto tank_texture = textures_pack_->get_texture(texture_id::tank_base);
            auto& texture_size = tank_texture->get_size();
            const auto tank_container = std::make_shared<bt::container>();
            //tank_container->setPosition(100.0, 100.0);
            tank_container->setOrigin(static_cast<float>(texture_size.x) / 2.0f, static_cast<float>(texture_size.y) / 2.0f);

            const auto tank_base_sprite = std::make_shared<bt::sprite>(tank_texture);
            tank_container->add_child(tank_base_sprite);

            auto turret_texture = textures_pack_->get_texture(texture_id::tank_turret);
            const auto turret_sprite = std::make_shared<bt::sprite>(turret_texture);
            turret_sprite->setPosition(16.5f, 27.0f);
            turret_sprite->setOrigin(10.0f, 67.0f);
            tank_container->add_child(turret_sprite);

            turret_render_object_ = turret_sprite;

            render_object_ = tank_container;
        }

        virtual void create_game_object_entity() override
        {
            auto player_entity = std::make_unique<bt::player_game_object_entity>(id_, ph_body_factory_);
            player_entity->create_phy_body();
            game_object_entity_ = std::move(player_entity);
        }

    private:
        float velocity_{ 0.0f };
        sf::Vector2f position_{};
        sf::Vector2u field_size_{};

        std::shared_ptr<bt::textures_pack> textures_pack_;
        std::weak_ptr<physics_body_factory> ph_body_factory_;

        std::weak_ptr<bt::sprite> turret_render_object_{};
    };
}
