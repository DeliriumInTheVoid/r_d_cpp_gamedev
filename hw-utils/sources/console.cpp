#include <iostream>

#include "../headers/console.hpp"

void utl::clear_console() {
#ifdef _WIN32
    std::system("cls");
#else
    std::cout << "\033[2J\033[1;1H";
#endif
}

void utl::wait_to_continue()
{
    std::cout << "\nPress Enter to continue" << std::flush;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
    clear_console();
}
