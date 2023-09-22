#pragma once

#include <string>
#include <memory>

#include <SFML/Graphics/Texture.hpp>

namespace bt
{
    class texture_render_data
    {
    public:
        explicit texture_render_data(const std::string& path)
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

        sf::Vector2u get_size() const
        {
            return texture_size_;
        }

    private:
        std::shared_ptr<sf::Texture> texture_{ nullptr };
        sf::Vector2u texture_size_{};
    };
}
