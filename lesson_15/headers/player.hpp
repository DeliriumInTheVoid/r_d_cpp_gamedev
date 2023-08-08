#pragma once

#include <string_view>

#include "team.hpp"
#include "weapon.hpp"

class team;

constexpr int player_max_weapons_num = 5;

enum class player_type : unsigned
{
    unknown = 0,
    assault,
    support,
    sniper,
    medic
};

class player
{
public:
    friend std::ostream& operator << (std::ostream& os, player& instance);
    friend class team;

public:
    player(std::string_view name, const player_type type, const float health);

public:
    void add_weapon(const weapon&& weapon);
    void print_player_info();

private:

    std::string_view name_;
    float health_{ 0 };
    player_type type_{ player_type::unknown };

    int weapon_slot_pt_{ 0 };
    weapon weapons_[player_max_weapons_num]{};

    team* team_ = nullptr;
};
