#pragma once

#include "renderer/container.hpp"


namespace bt
{
    class player_hp_text final : public bt::render_object
    {
    public:
        explicit player_hp_text(const sf::Font& font)
        {
            hp_text_.setFont(font);
            hp_text_.setCharacterSize(24);
            hp_text_.setFillColor(sf::Color::Green);
            hp_text_.setStyle(sf::Text::Bold);
        }

        virtual ~player_hp_text() override = default;

    public:
        void set_hit_point(const sf::Int32 hp)
        {
            hp_text_.setString(std::to_string(hp));
            set_text_color(hp_text_, hp);
        }

        sf::FloatRect get_local_bounds() const
        {
            return hp_text_.getLocalBounds();
        }

    protected:
        void draw(const std::shared_ptr<sf::RenderTarget>& render_target, const sf::Transform& parent_transform) const override
        {
            render_target->draw(hp_text_, parent_transform * getTransform());
        }

    private:
        void set_text_color(sf::Text& text, const sf::Int32 current_hp)
        {
            if (current_hp > 50)
            {
                text.setFillColor(sf::Color::Green);
            }
            else if (current_hp > 25)
            {
                text.setFillColor(sf::Color::Yellow);
            }
            else
            {
                text.setFillColor(sf::Color::Red);
            }
        }

    private:
        sf::Text hp_text_{};
    };

    class game_screen final : public bt::render_scene
    {
    public:
        explicit game_screen(const std::shared_ptr<sf::RenderTarget>& render_target)
            : render_scene(render_target)
        {
            font_.loadFromFile("game_data/fonts/wheaton capitals.otf");

            player_hp_text_ = std::make_shared<bt::player_hp_text>(font_);
            enemy_hp_text_ = std::make_shared<bt::player_hp_text>(font_);

            set_payer_hp(100);
            set_enemy_hp(100);

            player_hp_text_->setPosition(10, 10);
            enemy_hp_text_->setPosition(
                static_cast<float>(bt::game_entity_consts::game_field_size.x) - enemy_hp_text_->get_local_bounds().width
                - 15,
                10
            );

            add_child(player_hp_text_);
            add_child(enemy_hp_text_);
        }

        virtual ~game_screen() override = default;

    public:
        void set_payer_hp(const sf::Int32 hp) const
        {
            player_hp_text_->set_hit_point(hp);
        }

        void set_enemy_hp(const sf::Int32 hp) const
        {
            enemy_hp_text_->set_hit_point(hp);
        }

    private:
        std::shared_ptr<bt::player_hp_text> player_hp_text_{ nullptr };
        std::shared_ptr<bt::player_hp_text> enemy_hp_text_{ nullptr };

        sf::Font font_{};
    };
}
