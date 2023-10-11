#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "unordered_set"
#include "dev/debug_draw.hpp"
#include "game/game_objects/game_object_frame_restorer_packet.hpp"
#include "game_objects/forest_map_game_object.hpp"

#include "physics/collision_listener.hpp"
#include "game_objects/game_scene.hpp"
#include "game_states/game_connection_states.hpp"
#include "physics/physics_body_factory.hpp"
#include "renderer/render_scene.hpp"
#include "ui/game_screen.hpp"


namespace bt
{
    class sfml_game
    {
    public:
        sfml_game(const std::shared_ptr<sf::RenderWindow>& render_target, const float pixels_per_meters)
            : pixels_per_meters_{ pixels_per_meters }, debug_draw_{ render_target, pixels_per_meters_ }, window_{ render_target }
        {
            game_world_ = std::make_unique<bt::game_scene>();
            render_scene_ = std::make_unique<bt::render_scene>(render_target);
            game_screen_ = std::make_unique<bt::game_screen>(render_target);
            physics_world_.SetAllowSleeping(true);
            //physics_world.SetContinuousPhysics(true);
            contact_listener_ = std::make_unique<bt::collision_listener>();
            physics_world_.SetContactListener(contact_listener_.get());
            debug_draw_.AppendFlags(b2Draw::e_shapeBit);
            physics_world_.SetDebugDraw(&debug_draw_);

            contact_listener_->add_collision_handler([this](bt::game_object_b2d_link game_object_a, bt::game_object_b2d_link game_object_b)
                {
                    if (game_object_a.type == bt::game_object_type::bullet)
                    {
                        //objects_to_delete_.insert(game_object_a.id);
                    }
                    else if (game_object_b.type == bt::game_object_type::bullet)
                    {
                        //objects_to_delete_.insert(game_object_b.id);
                    }
                });

            physics_body_factory_ = std::make_shared<physics_body_factory>(&physics_world_);

            texture_warehouse_ = std::make_shared<bt::texture_warehouse>();
            const auto map_atlas = bt::atlas_data{
                bt::atlas_id::map_forest_tile_set,
        "game_data/atlases/map_forest_tileset.png",
        {
                    { bt::texture_id::grass_light_tl, sf::IntRect{1,1,16,16} },
                    { bt::texture_id::grass_light_tm, sf::IntRect{18,1,16,16} },
                    { bt::texture_id::grass_light_tr, sf::IntRect{35,1,16,16} },

                    { bt::texture_id::grass_light_lm, sf::IntRect{1,18,16,16} },
                    { bt::texture_id::grass_light_m, sf::IntRect{18,18,16,16} },
                    { bt::texture_id::grass_light_rm, sf::IntRect{35,18,16,16} },

                    { bt::texture_id::grass_light_bl, sf::IntRect{1,35,16,16} },
                    { bt::texture_id::grass_light_bm, sf::IntRect{18,35,16,16} },
                    { bt::texture_id::grass_light_br, sf::IntRect{35,35,16,16} },

                    { bt::texture_id::grass_light_dec_1, sf::IntRect{52,1,16,16} },
                    { bt::texture_id::grass_light_dec_2, sf::IntRect{69,1,16,16} },
                    { bt::texture_id::grass_light_dec_3, sf::IntRect{86,1,16,16} },

                    { bt::texture_id::grass_light_dec_4, sf::IntRect{52,18,16,16} },
                    { bt::texture_id::grass_light_dec_5, sf::IntRect{69,18,16,16} },
                    { bt::texture_id::grass_light_dec_6, sf::IntRect{86,18,16,16} },

                    { bt::texture_id::grass_light_dec_7, sf::IntRect{52,35,16,16} },
                    { bt::texture_id::grass_light_dec_8, sf::IntRect{69,35,16,16} },
                    { bt::texture_id::grass_light_dec_9, sf::IntRect{86,35,16,16} },

                    { bt::texture_id::grass_dark_tl, sf::IntRect{1,52,16,16} },
                    { bt::texture_id::grass_dark_tm, sf::IntRect{18,52,16,16} },
                    { bt::texture_id::grass_dark_tr, sf::IntRect{35,52,16,16} },

                    { bt::texture_id::grass_dark_lm, sf::IntRect{1,69,16,16} },
                    { bt::texture_id::grass_dark_m, sf::IntRect{18,69,16,16} },
                    { bt::texture_id::grass_dark_rm, sf::IntRect{35,69,16,16} },

                    { bt::texture_id::grass_dark_bl, sf::IntRect{1,86,16,16} },
                    { bt::texture_id::grass_dark_bm, sf::IntRect{18,86,16,16} },
                    { bt::texture_id::grass_dark_br, sf::IntRect{35,86,16,16} },
                }
            };

            texture_warehouse_->pre_load_atlas(map_atlas);

            const auto forest_game_map = std::make_shared<bt::forest_map_game_object>(
                0,
                texture_warehouse_->load_pack(bt::textures_pack_id::map_forest),
                bt::game_entity_consts::game_field_size,
                physics_body_factory_
            );
            forest_game_map->initialize();
            add_game_object(forest_game_map);

            const auto rock_texture_render_data = std::make_shared<bt::texture_holder>("game_data/atlases/rock_1.png");
            forest_game_map->create_rock({ 200, 200 }, rock_texture_render_data);

            game_session_ = std::make_shared<bt::game_session>();

            connection_state_manager_ = std::make_unique<bt::connection_state_manager>(
                game_session_,
                texture_warehouse_,
                physics_body_factory_,
                [this](const std::shared_ptr<bt::player_game_object>& player_game_object)
                {
                    add_game_object(player_game_object);
                    players_.emplace(player_game_object->get_id(), player_game_object);
                },
                [this](const std::shared_ptr<bt::game_object>& game_object)
                {
                    add_game_object(game_object);
                },
                [this](const bt::uuid& game_object_id)
                {
                    delete_game_object(game_object_id);
                    if (players_.contains(game_object_id))
                    {
                        players_.erase(game_object_id);
                    }
                },
                [this](const bt::uuid game_object_id, const bt::game_object_frame_restorer_packet& restorer_packet)
                {
                    game_world_->get_game_object(game_object_id)->restore_from_frame(restorer_packet);
                }
            );
            connection_state_manager_->start();
        }

        ~sfml_game()
        {
        }

    public:
        void add_game_object(const std::shared_ptr<bt::game_object>& game_object) const
        {
            game_world_->add_game_object(game_object);
            render_scene_->add_child(game_object->get_render_object());
        }

        void delete_game_object(const bt::uuid game_object_id)
        {
            objects_to_delete_.insert(game_object_id);
        }

        void update(const float delta_time)
        {
            for (const auto& player : players_ | std::views::values)
            {
                if (player->get_id() == game_session_->get_player_id())
                {
                    game_screen_->set_payer_hp(player->get_health());
                }
                else
                {
                    game_screen_->set_enemy_hp(player->get_health());
                }
            }

            window_->clear(sf::Color::Black);

            delete_game_objects();

            connection_state_manager_->update(delta_time);

            physics_world_.Step(delta_time, 8, 3);
            game_world_->update(delta_time);
            render_scene_->draw_scene();
            physics_world_.DebugDraw();

            game_screen_->draw_scene();
            window_->display();
        }

        void handle_event(const sf::Event& event) const
        {
            connection_state_manager_->handle_event(event);
        }

    private:
        void delete_game_objects()
        {
            for (const auto& game_object_id : objects_to_delete_)
            {
                auto game_object = game_world_->get_game_object(game_object_id);
                render_scene_->remove_child(game_object->get_render_object());
                game_world_->remove_game_object(game_object);
            }
            objects_to_delete_.clear();
        }

    private:
        float pixels_per_meters_{};
        b2World physics_world_{ b2Vec2{ 0.0f, 0.0f } };
        std::unordered_set<bt::uuid> objects_to_delete_{};
        SFMLDebugDraw debug_draw_;

        //TODO:: add more screens and screen management 
        std::unique_ptr<game_screen> game_screen_;

        std::shared_ptr<sf::RenderWindow> window_{ nullptr };
        std::unique_ptr<bt::game_scene> game_world_{ nullptr };
        std::unique_ptr<bt::render_scene> render_scene_{ nullptr };
        std::shared_ptr<bt::physics_body_factory> physics_body_factory_{ nullptr };
        std::unique_ptr<bt::collision_listener> contact_listener_{ nullptr };

        std::shared_ptr<bt::texture_warehouse> texture_warehouse_{ nullptr };
        std::shared_ptr<bt::game_session> game_session_{ nullptr };
        std::unordered_map<bt::uuid, std::shared_ptr<bt::player_game_object>> players_{};
        std::unique_ptr<bt::connection_state_manager> connection_state_manager_{ nullptr };
    };

}
