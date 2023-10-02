#pragma once

#include "renderer/textures.hpp"


namespace bt
{
    class forest_map_game_object : public bt::game_object
    {
    public:
        explicit forest_map_game_object(
            const bt::uuid id,
            const std::shared_ptr<textures_pack>& map_textures_pack,
            const sf::Vector2u& game_field_size,
            const std::weak_ptr<bt::physics_body_factory>& ph_body_factory
        )
            : game_object(id), map_textures_pack_(map_textures_pack), field_size_{ game_field_size },
            ph_body_factory_(ph_body_factory)
        {
        }

        virtual ~forest_map_game_object() override = default;
    public:
        void restore_from_frame(const bt::game_object_frame_restorer& restorer) override
        {
        }

        void update(float delta_time) override
        {
        }

    protected:
        void create_render_object() override
        {
            auto main_cont = std::make_shared<bt::container>();

            auto bg_texture = map_textures_pack_->get_texture(texture_id::grass_dark_m);
            bg_texture->get_texture()->setRepeated(true);

            //TODO:: build tiles map from config file
            //auto tile_size = sf::Vector2i{ bg_texture->get_texture_rect().width, bg_texture->get_texture_rect().height };
            //const int tiles_count_x = field_size_.x / bg_texture->get_texture_rect().width + 1;
            //const int tiles_count_y = field_size_.y / bg_texture->get_texture_rect().height + 1;

            //for (int i = 0; i < tiles_count_x; ++i)
            //{
            //    for (int j = 0; j < tiles_count_y; ++j)
            //    {
            //        auto bg_tile = std::make_shared<bt::sprite>(bg_texture);
            //        bg_tile->setPosition
            //        (sf::Vector2f{ static_cast<float>(i * bg_texture->get_texture_rect().width),
            //            static_cast<float>(j * bg_texture->get_texture_rect().height) }
            //        );
            //        main_cont->add_child(bg_tile);
            //    }
            //}

            render_object_ = main_cont;
        }

        void create_game_object_entity() override
        {
            auto map_entity = std::make_unique<game_map_entity>(
                0,
                b2Vec2{
                    static_cast<float>(field_size_.x) / physics_consts::pixels_per_meters,
                    static_cast<float>(field_size_.y) / physics_consts::pixels_per_meters
                },
                ph_body_factory_
            );
            map_entity->build_map();
            game_object_entity_ = std::move(map_entity);
        }

    private:
        std::shared_ptr<textures_pack> map_textures_pack_;
        sf::Vector2u field_size_;
        std::weak_ptr<bt::physics_body_factory> ph_body_factory_;
    };
}
