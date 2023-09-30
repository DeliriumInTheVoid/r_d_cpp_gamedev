#pragma once

#include <SFML/Config.hpp>


enum class player_action : sf::Uint32
{
    unknown = 0,
    move_forward,
    move_backward,
    stop_move,

    turn_left,
    turn_right,
    stop_turn,

    turn_turret_left,
    turn_turret_right,
    stop_turn_turret,

    turret_fire,
    turret_stop_fire,
};
