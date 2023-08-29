#pragma once

#include <utility>

#include "values.hpp"
#include "weapon/weapon_base.hpp"


class player_class
{
public:
    player_class(weapon_mastery&& weapon_mastery): weapon_mastery_ { weapon_mastery }
    {
    }

    virtual ~player_class() = default;

public:
    const weapon_mastery* get_weapon_mastery() const;

public:
    virtual float get_damage_multiplier() const = 0;
    virtual float get_defense_multiplier() const = 0;

private:
    weapon_mastery weapon_mastery_;
};

class default_class : public player_class
{
public:

    default_class() :default_class(
        {
            20,
            weapon_type::melee_edged,
            range{10, 40}
        })
    {
    }

    default_class(weapon_mastery weapon_mastery) : player_class(std::move(weapon_mastery))
    {
    }

public:
    virtual float get_damage_multiplier() const override;
    virtual float get_defense_multiplier() const override;
};

class defender_class final : public default_class
{
public:
    defender_class() : default_class(
        {
            20,
            weapon_type::ranged_shotgun,
            range{10, 40}
        })
    {
    }

public:
    virtual float get_defense_multiplier() const override;
};

class assault_class final : public default_class
{
public:
    assault_class() : default_class(
        {
            35,
            weapon_type::ranged_machine_gun,
            range{5, 30}
        })
    {
    }
};

class sniper_class final : public default_class
{
public:
    sniper_class() : default_class(
        {
            53,
            weapon_type::ranged_sniper_rifle,
            range{15, 20}
        })
    {
    }
};

class medic_class final : public default_class
{
public:
    medic_class() : default_class(
        {
            25,
            weapon_type::ranged_pistol,
            range{5, 10}
        })
    {
    }
};
