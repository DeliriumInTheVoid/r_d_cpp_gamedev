#include "player_class.hpp"

const weapon_mastery* player_class::get_weapon_mastery() const
{
    return &weapon_mastery_;
}

float default_class::get_damage_multiplier() const
{
    return 1.0f;
}

float default_class::get_defense_multiplier() const
{
    return 1.0f;
}

float defender_class::get_defense_multiplier() const
{
    return 1.2f;
}
