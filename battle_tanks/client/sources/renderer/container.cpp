#include "renderer/container.hpp"
#include "renderer/render_object.hpp"

void bt::container::add_child(const std::shared_ptr<render_object>& child)
{
    if (child->parent_ != nullptr)
    {
        child->parent_->remove_child(child);
    }

    children_.push_back(child);
    child->parent_ = this;
}

void bt::container::remove_child(const std::shared_ptr<render_object>& child)
{
    for (auto it = children_.begin(); it != children_.end(); ++it)
    {
        if (*it == child)
        {
            children_.erase(it);
            break;
        }
    }
}

void bt::container::remove_child(const render_object* const child)
{
    for (auto it = children_.begin(); it != children_.end(); ++it)
    {
        if (it->get() == child)
        {
            children_.erase(it);
            break;
        }
    }
}

void bt::container::draw(const std::shared_ptr<sf::RenderTarget>& render_target, const sf::Transform& parent_transform) const
{
    const sf::Transform transform = parent_transform * getTransform();
    for (const auto& ch : children_)
    {
        ch->draw(render_target, transform);
    }
}