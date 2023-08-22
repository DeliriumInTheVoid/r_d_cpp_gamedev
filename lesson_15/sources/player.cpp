#include "../headers/player.hpp"

#include <iostream>

std::ostream& operator << (std::ostream& os, const player_type instance)
{
    switch (instance)
    {
    case player_type::unknown:
        os << "unknown";
        break;
    case player_type::assault:
        os << "assault";
        break;
    case player_type::support:
        os << "support";
        break;
    case player_type::sniper:
        os << "sniper";
        break;
    case player_type::medic:
        os << "medic";
        break;
    }

    return os;
}

std::ostream& operator << (std::ostream& os, player& instance)
{
    os << "player {\n   name: " << instance.name_
        << ",\n   type: " << instance.type_
        << ",\n   health: " << instance.health_
        << ",\n   weapons: [\n";

    for (weapon& wep : instance.weapons_)
    {
        if (!wep.get_name().empty())
        {
            os << "      " << wep << "\n";
        }
    }

    os << "   ]";

    if (instance.team_ != nullptr)
    {
        os << ",\n   team: " << instance.team_->get_name();
    }

    os << "\n}";

    return os;
}

player::player(std::string_view name, const player_type type, const float health) :
    name_{ name }, health_{ health }, type_{ type }
{
}


void player::add_weapon(const weapon&& weapon)
{
    if (weapon_slot_pt_ >= player_max_weapons_num)
    {
        weapon_slot_pt_ = 0;
    }

    weapons_[weapon_slot_pt_] = weapon;
    ++weapon_slot_pt_;
}

void player::print_player_info()
{
    std::cout << *this;
}

void player::set_team(team* team)
{
    team_ = team;
}
