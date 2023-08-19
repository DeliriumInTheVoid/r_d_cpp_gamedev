#include <iostream>

#include "headers/game.hpp"

void print_menu();
void clear_console();

int main()
{
    std::srand(std::time(nullptr));
    std::cout << "Home Work 11\n";
    std::cout << "-----------\n\n";

    int user_choice{ 0 };
    while (user_choice != 3)
    {
        print_menu();
        std::cin >> user_choice;
        clear_console();
        switch (user_choice)
        {
        case 1:
        {
            play_daily_word();
            break;
        }
        case 2:
        {
            play_random_word();
            break;
        }
        case 3:
            std::cout << "Exit\n";
            break;
        default:
            std::cout << "Wrong choice!";
            user_choice = 0;
            break;
        }
        std::cout << "\nPress Enter to continue" << std::flush;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        clear_console();
    }

    return 0;
}

void print_menu()
{
    std::cout << "1. word of the day\n";
    std::cout << "2. random word\n";
    std::cout << "3. Exit\n\n";

    std::cout << "Enter number:";
}

void clear_console() {
#ifdef _WIN32
    std::system("cls");
#else
    std::cout << "\033[2J\033[1;1H";
#endif
}
