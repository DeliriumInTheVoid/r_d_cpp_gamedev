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
    };

    enum class textures_pack_id
    {
        unknown = 0,
        tank = 1,
    };

    class texture_holder
    {
    public:
        explicit texture_holder(const std::string& path)
        {
            texture_ = std::make_shared<sf::Texture>();
            texture_->loadFromFile(path);
            texture_->setSmooth(true);
            texture_size_ = texture_->getSize();
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

    private:
        std::shared_ptr<sf::Texture> texture_{ nullptr };
        sf::Vector2u texture_size_{};
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
        textures_pack(const bt::textures_pack_id id, const std::vector<texture_id>& textures_ids, const std::shared_ptr<texture_loader>& loader)
            : id_{ id }, textures_ids_{ textures_ids }, loader_{ loader }
        {
        }

        void load_pack()
        {
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
        std::shared_ptr<texture_loader> loader_{ nullptr };
        std::unordered_map<texture_id, std::shared_ptr<texture_holder>> loaded_textures_{};
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
        std::shared_ptr<textures_pack> load_pack(const textures_pack_id id)
        {
            if (!textures_packs_.contains(id))
            {
                const std::vector<texture_id>& textures_ids = get_textures_ids(id);
                textures_packs_.emplace(id, std::make_shared<textures_pack>(id, textures_ids, std::make_shared<texture_warehouse>()));
                textures_packs_.at(id)->load_pack();
            }
            
            return textures_packs_.at(id);
        }

        std::shared_ptr<texture_holder> get_texture(const textures_pack_id pack_id, const texture_id texture_id)
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
            static std::vector unknown_textures_ids{ texture_id::unknown };
            switch (id)
            {
            case textures_pack_id::tank:
                return tank_textures_ids;
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
    };
}
