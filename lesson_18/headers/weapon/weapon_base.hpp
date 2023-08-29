#pragma once

#include "values.hpp"


enum weapon_type : unsigned
{
    ranged_pistol = 1,
    ranged_sniper_rifle,
    ranged_machine_gun,
    ranged_shotgun,
    melee_edged,
    melee_pointed,
    melee_blunt
};

class weapon_mastery
{
public:
    weapon_mastery(const int chance, const weapon_type type, const range&& damage)
        : chance_{ chance }, type_{ type }, damage_ { damage }
    {
    }

    weapon_type get_type() const;
    int get_chance() const;
    int get_damage() const;

private:
    int chance_;
    weapon_type type_;
    range damage_;
};

class weapon
{
public:
    weapon(const chanced_range&& damage, const weapon_type type) : type_{ type }, damage_{ damage }
    {
    }

    virtual ~weapon() = default;

public:
    void set_mastery(const weapon_mastery* const mastery);
    weapon_type get_type() const;

    virtual int get_damage_rate(int distance) const = 0;

protected:
    int get_damage() const;

private:
    weapon_type type_;
    chanced_range damage_;
    const weapon_mastery* mastery_{ nullptr };
};
