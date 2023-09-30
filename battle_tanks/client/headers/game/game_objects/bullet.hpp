#pragma once

#include <memory>

#include <SFML/System/Vector2.hpp>
#include <box2d/b2_body.h>

#include "game/game_objects/game_object.hpp"
#include "renderer/textures.hpp"
#include "utils/uuid.hpp"


namespace bt
{
    //TODO:: create object poll for such objects
    class bullet : public bt::game_object
    {
    public:
        bullet(const bt::uuid id, float velocity, const std::shared_ptr<bt::texture_holder>& render_data, b2Body* body);

        virtual ~bullet() override = default;

    public:

        virtual void update(float delta_time) override;

        virtual void free() override;

    private:
        float velocity_{ 0.0f };
        sf::Vector2f position_{};
        sf::Vector2u field_size_{};

        std::shared_ptr<bt::texture_holder> render_data_{ nullptr };
    };
}
