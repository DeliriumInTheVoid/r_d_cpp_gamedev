#include <iostream>

#include "console.hpp"
#include "headers/rectangle.hpp"
#include "headers/player.hpp"
#include "headers/weapon.hpp"
#include "headers/team.hpp"

void print_menu();
void rectangle_class_test();
void composition_with_player_and_weapon();
void aggregation_with_player_and_team();

int main()
{
    std::cout << "Home Work 15\n";
    std::cout << "-----------\n\n";

    int user_choice{ 0 };
    while (user_choice != 4)
    {
        print_menu();
        std::cin >> user_choice;
        utl::clear_console();
        switch (user_choice)
        {
        case 1:
            rectangle_class_test();
            break;
        case 2:
            composition_with_player_and_weapon();
            break;
        case 3:
            aggregation_with_player_and_team();
            break;
        case 4:
            std::cout << "Exit\n";
            break;
        default:
            std::cout << "Wrong choice!";
            user_choice = 0;
            break;
        }
        utl::wait_to_continue();
    }

    return 0;
}


void print_menu()
{
    std::cout << "1. rectangle class usage\n";
    std::cout << "2. composition with player and weapon\n";
    std::cout << "3. aggregation with player and team\n";
    std::cout << "4. Exit\n\n";

    std::cout << "Enter number:";
}

void rectangle_class_test()
{
    const rectangle rect1{ 10.0, 2.5 };

    std::cout << "rectangle 10 x 2.5:\n\tperimeter = " << rect1.get_perimeter()
    << "\n\tarea = " << rect1.get_area() << std::endl;


    const rectangle rect2{ 7.0, 5.0 };
    std::cout << "rectangle 7 x 5:\n\tperimeter = " << rect2.get_perimeter()
        << "\n\tarea = " << rect2.get_area() << std::endl;
}

void composition_with_player_and_weapon()
{
    player pl1{ "player1", player_type::assault, 100 };
    std::cout << pl1 << std::endl;

    pl1.add_weapon(weapon{ "AK-47", 30, 200 });
    pl1.add_weapon(weapon{ "M4A1", 100, 180 });
    pl1.add_weapon(weapon{ "knife", 50, 1 });
    std::cout << pl1 << std::endl;

    player pl2{ "player2", player_type::sniper, 80 };
    pl2.add_weapon(weapon{ "Sniper Rifle", 70, 500 });
    std::cout << pl2 << std::endl;
}

void aggregation_with_player_and_team()
{
    player pl1{ "player1", player_type::assault, 100 };
    pl1.add_weapon(weapon{ "AK-47", 30, 200 });

    player pl2{ "player2", player_type::sniper, 80 };
    pl2.add_weapon(weapon{ "Sniper Rifle", 70, 500 });

    player pl3{ "player3", player_type::assault, 80 };
    pl3.add_weapon(weapon{ "M4A1", 100, 180 });

    player pl4{ "player4", player_type::medic, 50 };
    pl4.add_weapon(weapon{ "knife", 50, 1 });

    team team{ "A team" };

    team.add_player(&pl1);
    team.add_player(&pl2);
    team.add_player(&pl3);
    team.add_player(&pl4);

    std::cout << pl1 << std::endl;
    std::cout << pl2 << std::endl;
    std::cout << pl3 << std::endl;
    std::cout << pl4 << std::endl;

    std::cout << "\n\nRemove player1 from \"A team\"\n" << std::endl;
    team.remove_player(&pl1);
    pl1.print_player_info();
    std::cout << std::endl;

    std::cout << std::endl;
}
