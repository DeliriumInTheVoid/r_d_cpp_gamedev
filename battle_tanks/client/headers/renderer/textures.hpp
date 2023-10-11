#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include <SFML/Graphics/Texture.hpp>

namespace bt
{
	enum class texture_id
    {
        unknown = 0,
        tank_base = 1,
        tank_turret = 2,
        bullet = 3,

        //atlas 
        grass_light_m,
        grass_light_tl,
        grass_light_tm,
        grass_light_tr,
        grass_light_bl,
        grass_light_bm,
        grass_light_br,
        grass_light_lm,
        grass_light_rm,

        grass_light_dec_1,
        grass_light_dec_2,
        grass_light_dec_3,
        grass_light_dec_4,
        grass_light_dec_5,
        grass_light_dec_6,
        grass_light_dec_7,
        grass_light_dec_8,
        grass_light_dec_9,

        grass_dark_m,
        grass_dark_tl,
        grass_dark_tm,
        grass_dark_tr,
        grass_dark_bl,
        grass_dark_bm,
        grass_dark_br,
        grass_dark_lm,
        grass_dark_rm,
    };

    enum class textures_pack_id
    {
        unknown = 0,
        tank,
        map_forest,
    };

    enum class atlas_id
    {
        unknown = 0,
        map_forest_tile_set,
    };

    class texture_holder
    {
    public:
        explicit texture_holder(const std::shared_ptr<sf::Texture>& atlas_texture, const sf::IntRect& texture_rect)
        {
            texture_ = atlas_texture;
            texture_size_ = texture_->getSize();
            texture_rect_ = texture_rect;
        }

        explicit texture_holder(const std::string& path)
        {
            texture_ = std::make_shared<sf::Texture>();
            texture_->loadFromFile(path);
            texture_->setSmooth(true);
            texture_size_ = texture_->getSize();
            texture_rect_ = { 0,0,
                static_cast<int>(texture_size_.x),
                static_cast<int>(texture_size_.y)
            };
        }

    public:
        std::shared_ptr<sf::Texture> get_texture() const
        {
            return texture_;
        }

        const sf::Vector2u& get_size() const
        {
            return texture_size_;
        }

        sf::IntRect get_texture_rect() const
        {
            return texture_rect_;
        }

    private:
        std::shared_ptr<sf::Texture> texture_{ nullptr };
        sf::Vector2u texture_size_{};
        sf::IntRect texture_rect_;
    };

    class texture_loader
    {
    public:
        virtual ~texture_loader() = default;
    public:
        virtual std::shared_ptr<texture_holder> load_texture(texture_id id) = 0;
    };

    class textures_pack
    {
    public:
        textures_pack(const bt::textures_pack_id id, const std::vector<texture_id>& textures_ids, texture_loader* loader)
            : id_{ id }, textures_ids_{ textures_ids }, loader_{ loader }
        {
        }

        void load_pack()
        {
            if (loader_ == nullptr)
            {
	            return;
            }
            for (const auto& texture_id : textures_ids_)
            {
                loaded_textures_.emplace(texture_id, loader_->load_texture(texture_id));
            }
        }

        std::shared_ptr<texture_holder> get_texture(const texture_id id)
        {
            if (!loaded_textures_.contains(id))
            {
                return nullptr;
            }
            return loaded_textures_.at(id);
        }

        bool has_texture(const texture_id id) const
        {
            return loaded_textures_.contains(id);
        }

    public:
        textures_pack_id get_id() const
        {
            return id_;
        }

    private:
        bt::textures_pack_id id_{ bt::textures_pack_id::unknown };
        std::vector<texture_id> textures_ids_{};
        texture_loader* loader_{ nullptr };
        std::unordered_map<texture_id, std::shared_ptr<texture_holder>> loaded_textures_{};
    };

    class atlas_data
    {
    public:
        //TODO:: move atlas data to config file and create atlas_data from it
        atlas_data(
            const atlas_id id,
            std::string&& atlas_path,
            const std::unordered_map<texture_id, sf::IntRect>& textures_rect)
            : id_(id), textures_rect_(textures_rect), atlas_path_(std::move(atlas_path))
        {
        }

        ~atlas_data() = default;

    public:
        const std::string& get_atlas_path() const
        {
            return atlas_path_;
        }

        const std::unordered_map<texture_id, sf::IntRect>& get_textures_rect() const
        {
            return textures_rect_;
        }

        atlas_id get_id() const
        {
            return id_;
        }

    private:
        atlas_id id_;
        std::unordered_map<texture_id, sf::IntRect> textures_rect_;
        std::string atlas_path_;
    };

    class texture_warehouse : public texture_loader
    {
    public:
        texture_warehouse() = default;
        virtual ~texture_warehouse() override = default;

    public:
        virtual std::shared_ptr<texture_holder> load_texture(texture_id id) override
        {
            if (!loaded_textures_.contains(id))
            {
                loaded_textures_.emplace(id, std::make_shared<texture_holder>(get_texture_path(id)));
            }
            return loaded_textures_.at(id);
        }

    public:
        void pre_load_atlas(const atlas_data& atlas_data)
        {
            const auto& textures_rect = atlas_data.get_textures_rect();
            const auto atlas_texture = std::make_shared<sf::Texture>();
            atlas_texture->loadFromFile(atlas_data.get_atlas_path());

            for (const auto& [texture_id, texture_rect] : textures_rect)
            {
                if (!loaded_textures_.contains(texture_id))
                {
                    loaded_textures_.emplace(texture_id, std::make_shared<texture_holder>(atlas_texture, texture_rect));
                }
            }
        }

        std::shared_ptr<textures_pack> load_pack(const textures_pack_id id)
        {
            if (!textures_packs_.contains(id))
            {
                const std::vector<texture_id>& textures_ids = get_textures_ids(id);
                textures_packs_.emplace(id, std::make_shared<textures_pack>(id, textures_ids, this));
                textures_packs_.at(id)->load_pack();
            }
            
            return textures_packs_.at(id);
        }

        std::shared_ptr<texture_holder> get_texture(const textures_pack_id pack_id, const texture_id texture_id) const
        {
            if (!textures_packs_.contains(pack_id))
            {
                return nullptr;
            }
            return textures_packs_.at(pack_id)->get_texture(texture_id);
        }

        bool has_texture(const textures_pack_id pack_id, const texture_id texture_id) const
        {
            if (!textures_packs_.contains(pack_id))
            {
                return false;
            }
            return textures_packs_.at(pack_id)->has_texture(texture_id);
        }

    private:
        //TODO:: move to config file
        static std::vector<texture_id>& get_textures_ids(const textures_pack_id id)
        {
            static std::vector tank_textures_ids{ texture_id::tank_base, texture_id::tank_turret };
            static std::vector bullet_textures_ids{ texture_id::bullet };
            static std::vector map_forest_texture_ids{
                texture_id::grass_light_m,
                texture_id::grass_light_tl,
                texture_id::grass_light_tm,
                texture_id::grass_light_tr,
                texture_id::grass_light_bl,
                texture_id::grass_light_bm,
                texture_id::grass_light_br,
                texture_id::grass_light_lm,
                texture_id::grass_light_rm,

                texture_id::grass_light_dec_1,
                texture_id::grass_light_dec_2,
                texture_id::grass_light_dec_3,
                texture_id::grass_light_dec_4,
                texture_id::grass_light_dec_5,
                texture_id::grass_light_dec_6,
                texture_id::grass_light_dec_7,
                texture_id::grass_light_dec_8,
                texture_id::grass_light_dec_9,

                texture_id::grass_dark_m,
                texture_id::grass_dark_tl,
                texture_id::grass_dark_tm,
                texture_id::grass_dark_tr,
                texture_id::grass_dark_bl,
                texture_id::grass_dark_bm,
                texture_id::grass_dark_br,
                texture_id::grass_dark_lm,
                texture_id::grass_dark_rm,
            };
            static std::vector unknown_textures_ids{ texture_id::unknown };
            switch (id)
            {
            case textures_pack_id::tank:
                return tank_textures_ids;
            case textures_pack_id::map_forest:
                return map_forest_texture_ids;
            case textures_pack_id::unknown:
                return unknown_textures_ids;
            default:
                return unknown_textures_ids;
            }
        }

        static std::string get_texture_path(const texture_id id)
        {
            switch (id)
            {
            case texture_id::tank_base:
                return "game_data/atlases/tank.png";
            case texture_id::tank_turret:
                return "game_data/atlases/tower.png";
            case texture_id::bullet:
                return "game_data/atlases/bullet_1.png";
            case texture_id::unknown:
            default:
                return "";
            }
        }

    private:
        std::unordered_map<textures_pack_id, std::shared_ptr<textures_pack>> textures_packs_{};
        std::unordered_map<texture_id, std::shared_ptr<texture_holder>> loaded_textures_{};
        std::weak_ptr<texture_loader> loader_;
    };
}
