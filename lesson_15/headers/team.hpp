#pragma once

#include <string_view>

#include "player.hpp"

class player;

constexpr int team_max_players_num = 10;

class team
{
public:
    team(std::string_view name);

public:
    std::string_view get_name() const;

    void add_player(player* player);
    void remove_player(player* player);

private:
    std::string_view name_{};
    player* players_[team_max_players_num]{};
    int num_players_{ 0 };
};
