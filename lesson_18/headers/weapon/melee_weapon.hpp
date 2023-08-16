#pragma once

#include <utility>

#include "values.hpp"
#include "weapon/weapon_base.hpp"


class melee_weapon : public weapon
{
public:
    melee_weapon(const chanced_diapason_value&& damage,
        const weapon_type type,
        const int max_distance,
        chanced_value&& crit_modifier) : weapon(std::move(damage), type),
        max_distance_(max_distance), crit_{ crit_modifier }
    {
    }

public:
    virtual int get_damage_rate(int distance) const override;

private:
    float get_crit_multiplier() const;

private:
    int max_distance_;
    chanced_value crit_;
};

class bare_hand final : public melee_weapon
{
public:
    bare_hand() : melee_weapon(chanced_diapason_value{ 80, {1, 5} }, weapon_type::melee_blunt, 1, {80,3})
    {
    }
};

class axe final : public melee_weapon
{
public:
    axe() : melee_weapon(chanced_diapason_value{ 70, {25, 30} }, weapon_type::melee_edged, 2, { 20,5 })
    {
    }
};

class knife final : public melee_weapon
{
public:
    knife() : melee_weapon(chanced_diapason_value{ 75, {10, 20} }, weapon_type::melee_edged, 1, { 30,2 })
    {
    }
};
