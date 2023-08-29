#include "weapon/ranged_weapon.hpp"

int ranged_weapon::get_damage_rate(const int distance) const
{
    return static_cast<int>(get_precision(distance) * static_cast<float>(get_damage()));
}


float sniper_rifle::get_precision(const int distance) const
{
    if (distance > 2000)
    {
        return 0.2f;
    }
    if (distance > 1000)
    {
        return 1.5f;
    }
    if (distance > 300)
    {
        return 5.0f;
    }

    return 1.0f;
}

float pistol::get_precision(const int distance) const
{
    if (distance > 1000)
    {
        return 0.3f;
    }
    if (distance > 500)
    {
        return 0.5f;
    }
    if (distance > 20)
    {
        return 2.0f;
    }

    return 1.0f;
}

float shotgun::get_precision(const int distance) const
{

    if (distance > 300)
    {
        return 0.2f;
    }
    if (distance > 100)
    {
        return 0.8f;
    }
    if (distance > 20)
    {
        return 3.0f;
    }

    return 1.0f;
}

float assault_rifle::get_precision(const int distance) const
{
    if (distance > 1000)
    {
        return 0.1f;
    }
    if (distance > 700)
    {
        return 0.8f;
    }
    if (distance > 100)
    {
        return 2.5f;
    }

    return 1.0f;
}
