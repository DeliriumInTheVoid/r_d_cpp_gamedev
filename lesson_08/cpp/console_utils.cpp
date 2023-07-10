#include "../headers/console_utils.hpp"

void print_menu()
{
	std::cout << "1. count numbers by sign\n";
	std::cout << "2. factorial\n";
	std::cout << "3. natural numbers\n";
	std::cout << "4. Exit\n\n";

	std::cout << "Enter number:";
}

void clear_console() {
#ifdef _WIN32
	std::system("cls");
#else
	std::cout << "\033[2J\033[1;1H";
#endif
}
