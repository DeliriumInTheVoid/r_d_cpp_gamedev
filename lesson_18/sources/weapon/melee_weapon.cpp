#include "weapon/melee_weapon.hpp"

int melee_weapon::get_damage_rate(const int distance) const
{
    if (distance > max_distance_)
    {
        return 0;
    }

    return static_cast<int>(static_cast<float>(get_damage()) * get_crit_multiplier());
}

float melee_weapon::get_crit_multiplier() const
{
    const float crit_value = crit_.get_value();
    return crit_value > 0 ? crit_value : 1;
}
