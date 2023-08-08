#include "../headers/team.hpp"

team::team(std::string_view name) : name_{ name }
{
}

std::string_view team::get_name() const
{
    return name_;
}

void team::add_player(player* player)
{
    if (num_players_ < team_max_players_num)
    {
        players_[num_players_++] = player;
        player->team_ = this;
    }
    else
    {
        std::cerr << "Error! Team " << name_ << " is already full" << std::endl;
    }
}

void team::remove_player(player* player)
{
    for (int i = 0; i < num_players_; ++i)
    {
        if (players_[i] == player)
        {
            players_[i]->team_ = nullptr;
            for (int j = i; j < num_players_ - 1; ++j)
            {
                players_[j] = players_[j + 1];
            }
            players_[--num_players_] = nullptr;
            break;
        }
    }
}
