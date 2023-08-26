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
        player->set_team(this);
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
	        const int last_index = num_players_ - 1;
            if (num_players_ > 1 && i < last_index)
            {
                std::swap(players_[i], players_[last_index]);
            }
            players_[last_index]->set_team(nullptr);
            players_[--num_players_] = nullptr;
            break;
        }
    }
}
