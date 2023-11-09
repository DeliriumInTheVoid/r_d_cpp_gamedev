#pragma once

#include <memory>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace bt
{
    class container;

    class render_object : public sf::Transformable
    {
        friend bt::container;
    public:
        virtual ~render_object() override = default;

    public:
        virtual void free();

        //void set_parent(const std::shared_ptr<container>& parent)
        //{
        //    parent_ = parent;
        //}

        //std::shared_ptr<container> get_parent() const
        //{
        //    return parent_;
        //}

    protected:
        virtual void draw(const std::shared_ptr<sf::RenderTarget>& render_target, const sf::Transform& parent_transform) const = 0;

    private:
        bt::container* parent_{ nullptr };
    };
}
