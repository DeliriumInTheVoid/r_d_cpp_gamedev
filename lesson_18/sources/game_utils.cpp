#include <iostream>
#include <string>

#include "game_utils.hpp"


player* get_player_from_user()
{
    std::cout << "Enter Player name:\n";
    std::string name{};
    std::getline(std::cin, name);

    std::cout << std::endl;
    weapon* weapon{ get_weapon() };
    
    std::cout << std::endl;
    player_class* player_class{ get_player_class()};
    std::cout << std::endl;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    player* user_player = new player{name, weapon, player_class};
    return user_player;
}

weapon* get_weapon()
{
    unsigned weapon_type{ 0 };

    std::cout << "Pick a weapon for your player: \n";
    std::cout << "1. Shotgun \n";
    std::cout << "2. Sniper Rifle \n";
    std::cout << "3. Pistol \n";
    std::cout << "4. Assault Rifle \n";
    std::cout << "5. bare_hand \n";
    std::cout << "6. Axe \n";
    std::cout << "7. Knife \n";
    std::cout << std::endl;

    std::cin >> weapon_type;

    if (weapon_type < 1 || weapon_type > 7)
    {
        std::cerr << "Wrong choice. Bare Hand will be set\n" << std::endl;
    }

    weapon* player_weapon{ nullptr };

    switch (weapon_type)
    {
    case 1:
        player_weapon = new shotgun();
        break;
    case 2:
        player_weapon = new sniper_rifle();
        break;
    case 3:
        player_weapon = new pistol();
        break;
    case 4:
        player_weapon = new assault_rifle();
        break;
    case 5:
        player_weapon = new bare_hand();
        break;
    case 6:
        player_weapon = new axe();
        break;
    case 7:
        player_weapon = new knife();
        break;
    default:
        player_weapon = new bare_hand();
    }

    return player_weapon;
}

player_class* get_player_class()
{
    unsigned player_class_type{ 0 };

    std::cout << "Pick a class for your player: \n\n";
    std::cout << "1. Default Class \n";
    std::cout << "2. Defender Class \n";
    std::cout << "3. Assault Class \n";
    std::cout << "4. Sniper Class \n";
    std::cout << "5. Medic Class \n";
    std::cout << std::endl;

    std::cin >> player_class_type;

    if (player_class_type < 1 || player_class_type > 5)
    {
        std::cerr << "Wrong choice. Default Class will be set\n" << std::endl;
    }

    player_class* user_player_class{ nullptr };

    switch (player_class_type)
    {
    case 1:
        user_player_class = new default_class();
        break;
    case 2:
        user_player_class = new defender_class();
        break;
    case 3:
        user_player_class = new assault_class();
        break;
    case 4:
        user_player_class = new sniper_class();
        break;
    case 5:
        user_player_class = new medic_class();
        break;
    default:
        user_player_class = new default_class();
    }

    return user_player_class;
}
