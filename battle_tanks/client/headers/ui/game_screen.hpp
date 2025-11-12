#pragma once

#include <nlohmann/json.hpp>

#include "renderer/container.hpp"


namespace bt
{
    class game_screen_config final
    {
    public:
        game_screen_config()
        {
            nlohmann::json config_json;
            std::ifstream config_file("game_data/screens/game_screen_config.json");
            config_file >> config_json;

            wounded_threshold_ = config_json["hp_text_color_thresholds"]["wounded"].get<unsigned>();
            dying_threshold_ = config_json["hp_text_color_thresholds"]["dying"].get<unsigned>();
        }

        ~game_screen_config() = default;

    public:
        unsigned get_wounded_threshold() const
        {
            return wounded_threshold_;
        }

        unsigned get_dying_threshold() const
        {
            return dying_threshold_;
        }

    private:
        unsigned wounded_threshold_{};
        unsigned dying_threshold_{};
    };

    class player_hp_text final : public bt::render_object
    {
    public:
        explicit player_hp_text(const sf::Font& font, const unsigned wounded_threshold, const unsigned dying_threshold)
            : wounded_threshold_(wounded_threshold), dying_threshold_(dying_threshold)
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
        unsigned wounded_threshold_;
        unsigned dying_threshold_;
    };

    class game_screen final : public bt::render_scene
    {
    public:
        explicit game_screen(const std::shared_ptr<sf::RenderTarget>& render_target)
            : render_scene(render_target)
        {
            font_.loadFromFile("game_data/fonts/wheaton capitals.otf");
            config_ = std::make_unique<game_screen_config>();

            player_hp_text_ = std::make_shared<bt::player_hp_text>(font_, config_->get_wounded_threshold(), config_->get_dying_threshold());
            enemy_hp_text_ = std::make_shared<bt::player_hp_text>(font_, config_->get_wounded_threshold(), config_->get_dying_threshold());

            set_payer_hp(100);
            set_enemy_hp(100);

            player_hp_text_->setPosition(10, 10);
            enemy_hp_text_->setPosition(
                static_cast<float>(bt::game_entity_consts::game_field_size.x) - enemy_hp_text_->get_local_bounds().width - 15,
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
        std::unique_ptr<game_screen_config> config_{ nullptr };

        sf::Font font_{};
    };
}
