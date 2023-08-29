#include <iostream>
#include <string>

#include "player.hpp"

player::player(const std::string& name, weapon* weapon, player_class* player_class)
    : name_(name), class_(player_class), weapon_(weapon)
{
    if (weapon_ != nullptr && player_class != nullptr)
    {
        weapon_->set_mastery(player_class->get_weapon_mastery());
    }
}

std::string player::get_name() const
{
    return name_;
}

int player::get_base_damage(const int distance) const
{
    return weapon_->get_damage_rate(distance);
}

void player::set_health(int health)
{
    health_ = health;
}

bool player::is_eliminated() const
{
    return health_ <= 0;
}

void player::set_weapon(weapon* weapon)
{
    delete weapon_;
    weapon_ = weapon;
}

void player::set_player_class(player_class* player_class)
{
    delete class_;
    class_ = player_class;
}

void player::attack(player& other, int distance)
{
    if (&other == this)
    {
        std::cout << "\n-------FRIENDLY FIRE!-------\n";
    }

    if (is_eliminated())
    {
        std::cout << "Player " << get_name() << " can not attack. One has been already eliminated!\n";
        return;
    }

    if (other.is_eliminated())
    {
        std::cout << "Player " << other.get_name() << " has been already eliminated!\n";
        return;
    }

    const int damage = static_cast<int>(
        static_cast<float>(weapon_->get_damage_rate(distance)) * class_->get_damage_multiplier() / other.class_->get_defense_multiplier()
        );

    if (damage > 0)
    {
        other.health_ -= damage;
        std::cout << "Player " << get_name() << " attacked Player " << other.get_name() <<
            " with damage: " << damage << ", distance: " << distance << std::endl;

        std::cout << other.get_name() << " health left: " << other.health_ << std::endl;

        if (other.health_ <= 0)
        {
            other.health_ = 0;
            std::cout << "Player " << other.get_name() << " was eliminated!\n";
        }
    }
    else
    {
        std::cout << "Player " << get_name() << " missed Player " << other.get_name() << std::endl;
    }
    
}
