#pragma once

#include <memory>

#include <SFML/Graphics/RenderTarget.hpp>

#include "renderer/container.hpp"

namespace bt
{
    class render_scene : public bt::container
    {
    public:
        explicit render_scene(const std::shared_ptr<sf::RenderTarget>& render_target) : render_target_{ render_target }
        {
        }

        virtual ~render_scene() override = default;

    public:
        void draw_scene() const
        {
            bt::container::draw(render_target_, getTransform());
        }

    private:
        std::shared_ptr<sf::RenderTarget> render_target_{ nullptr };
    };
}