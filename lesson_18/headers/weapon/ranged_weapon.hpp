#pragma once

#include <utility>

#include "values.hpp"
#include "weapon/weapon_base.hpp"


class ranged_weapon : public weapon
{
public:
    ranged_weapon(const chanced_range&& damage, const weapon_type type): weapon(std::move(damage), type)
    {
    }

    int get_damage_rate(const int distance) const override;

protected:

    virtual float get_precision(int distance) const
    {
        return 1.0f;
    }
};

class sniper_rifle final : public ranged_weapon
{
public:
    sniper_rifle() : ranged_weapon(chanced_range{45, {30, 45}}, weapon_type::ranged_sniper_rifle)
    {
    }

protected:
    virtual float get_precision(const int distance) const override;
};

class pistol final : public ranged_weapon
{
public:
    pistol() : ranged_weapon(chanced_range{ 70, {15, 25} }, weapon_type::ranged_pistol)
    {
    }

protected:
    virtual float get_precision(const int distance) const override;
};

class shotgun final : public ranged_weapon
{
public:
    shotgun() : ranged_weapon(chanced_range{ 75, {10, 30} }, weapon_type::ranged_shotgun)
    {
    }

protected:
    virtual float get_precision(const int distance) const override;
};


class assault_rifle final : public ranged_weapon
{
public:
    assault_rifle() : ranged_weapon(chanced_range{ 62, {15, 20} }, weapon_type::ranged_machine_gun)
    {
    }

protected:
    virtual float get_precision(const int distance) const override;
};
