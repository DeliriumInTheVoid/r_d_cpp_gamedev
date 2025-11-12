#include <iostream>
#include <chrono>
#include <memory>

#include "SFML/System/Clock.hpp"
#include "SFML/Graphics.hpp"

#include "game/game_objects/player_game_object.hpp"
#include "game/sfml_game.hpp"
#include "network/player_actions.hpp"


int main()
{
    auto window = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(
            bt::game_entity_consts::game_field_size.x,
            bt::game_entity_consts::game_field_size.y
        ),
        "Battle Tanks"
    );
    //window->setFramerateLimit(60);

    sf::Clock clock;

    const auto game = std::make_unique<bt::sfml_game>(
        window,
        bt::physics_consts::pixels_per_meters
    );

    while (window->isOpen())
    {
        const float delta_time = clock.restart().asSeconds();
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Escape))
            {
                window->close();
            }
            game->handle_event(event);
        }

        game->update(delta_time);
    }

    return 0;
}
