#include "weapon/weapon_base.hpp"


weapon_type weapon_mastery::get_type() const
{
    return type_;
}

int weapon_mastery::get_chance() const
{
    return chance_;
}

int weapon_mastery::get_damage() const
{
    return damage_.generate_value();
}

void weapon::set_mastery(const weapon_mastery* const mastery)
{
    mastery_ = mastery->get_type() == type_ ? mastery : nullptr;
}

weapon_type weapon::get_type() const
{
    return type_;
}

int weapon::get_damage() const
{
    if (mastery_ != nullptr)
    {
        const int damage = damage_.get_value(mastery_->get_chance());
        return damage > 0 ? damage + mastery_->get_damage() : damage;
    }

    return damage_.get_value();
}
