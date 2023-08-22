#include "../headers/weapon.hpp"

std::ostream& operator << (std::ostream& os, weapon& instance)
{
    os << "weapon { name: " << instance.name_ << ", damage: " << instance.damage_ << ", range: " << instance.range_ << " }";
    return os;
}

weapon::weapon(std::string_view name, const float damage, const float range) :
    name_{ name }, damage_{ damage }, range_{ range }
{
}

std::string_view weapon::get_name() const
{
    return name_;
}

float weapon::get_damage() const
{
    return damage_;
}

float weapon::get_range() const
{
    return range_;
}
