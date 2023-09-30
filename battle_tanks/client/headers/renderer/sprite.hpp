#pragma once

#include <SFML/Graphics/Sprite.hpp>

#include "renderer/textures.hpp"
#include "renderer/render_object.hpp"

namespace bt
{
    class sprite : public bt::render_object
    {
    public:
        explicit sprite(const std::string& path)
        {
            texture_render_data_ = std::make_shared<texture_holder>(path);
            sprite_.setTexture(*texture_render_data_->get_texture());
        }

        explicit sprite(const std::shared_ptr<texture_holder>& texture_render_data)
        {
            texture_render_data_ = texture_render_data;
            sprite_.setTexture(*texture_render_data_->get_texture());
        }

        virtual ~sprite() override = default;

    protected:
        void draw(const std::shared_ptr<sf::RenderTarget>& render_target, const sf::Transform& parent_transform) const override
        {
            if (texture_render_data_ == nullptr)
            {
                return;
            }
            render_target->draw(sprite_, parent_transform * getTransform());
        }

    private:
        sf::Sprite sprite_{};
        std::shared_ptr<texture_holder> texture_render_data_{ nullptr };
    };
}
