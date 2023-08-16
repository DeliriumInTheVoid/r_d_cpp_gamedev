#pragma once

#include <string>

#include "player_class.hpp"
#include "weapon.hpp"


class player
{
public:
    player(const std::string& name = "Auto Generated NameId", weapon* weapon = new bare_hand, player_class* player_class = new default_class);

    ~player()
    {
        delete class_;
        delete weapon_;
    }

public:
    std::string get_name() const;
    int get_base_damage(const int distance) const;
    void set_health(int health);
    bool is_eliminated() const;
    void set_weapon(weapon* weapon);
    void set_player_class(player_class* player_class);
    void attack(player& other, int distance);

private:
    int health_ = 300;
    std::string name_;

    player_class* class_ = nullptr;
    weapon* weapon_ = nullptr;
};
