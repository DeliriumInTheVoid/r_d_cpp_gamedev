#pragma once

#include <memory>
#include <vector>

#include <SFML/Graphics/RenderTarget.hpp>
#include "SFML/Graphics/Transform.hpp"

#include "render_object.hpp"

class container : public render_object
{
public:
    virtual ~container() override = default;

public:
    void add_child(const std::shared_ptr<render_object>& child);

    void remove_child(const std::shared_ptr<render_object>& child);
    void remove_child(const render_object* const child);

protected:
    void draw(const std::shared_ptr<sf::RenderTarget>& render_target, const sf::Transform& parent_transform) const override;

protected:
    std::vector<std::shared_ptr<render_object>> children_{};
};