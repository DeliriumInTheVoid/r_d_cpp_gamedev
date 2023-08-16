#include <iostream>

#include "game_utils.hpp"
#include "values.hpp"
#include "player.hpp"


int main()
{
    std::srand(std::time(nullptr));


    player* player1 = get_player_from_user();
    player* player2 = get_player_from_user();

    const diapason_value range{ 1, 1000 };
    while (!player1->is_eliminated() && !player2->is_eliminated())
    {
        player1->attack(*player2, range.generate_value());
        player2->attack(*player1, range.generate_value());
    }

    delete player1;
    delete player2;

    return 0;
}
