#include <iostream>

#include "headers/console_utils.hpp"
#include "headers/cyclic_functions.hpp"
#include "headers/recursive_functions.hpp"

void factorial();
void natural_numbers();

int main()
{
	std::cout << "Home Work 8\n";
	std::cout << "-----------\n\n";

	int user_choice{ 0 };
	while (user_choice != 4)
	{
		print_menu();
		std::cin >> user_choice;
		clear_console();
		switch (user_choice)
		{
		case 1:
			count_numbers_by_sign();
			break;
		case 2:
			factorial();
			break;
		case 3:
			natural_numbers();
			break;
		case 4:
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

void factorial()
{
	int n{ 0 };

	std::cout << "Enter number:\n";
	std::cin >> n;

	if (n < 0 || n > 25) {
		std::cout << "number must be 0..25" << std::endl;
		return;
	}

	std::cout << "Factorial value : \na. with cycle: " << factorial_cycle(n) \
			<<"\nb. with recursion: " << factorial_recursion(n) << std::endl;
}

void natural_numbers()
{
	int n{ 0 };

	std::cout << "Enter number:\n";
	std::cin >> n;

	if (n < 1) {
		std::cout << n <<" is not natural number" << std::endl;
		return;
	}

	std::cout << std::endl;

	std::cout << "a. descending with cycle\n";
	print_natural_numbers_cycle_descending(n);
	std::cout << std::endl << std::endl;

	std::cout << "b. ascending with cycle\n";
	print_natural_numbers_cycle_ascending(n);
	std::cout << std::endl << std::endl;

	std::cout << "c. descending with recurse\n";
	print_natural_numbers_recurse_descending(n);
	std::cout << std::endl << std::endl;

	std::cout << "d. ascending with recurse\n";
	print_natural_numbers_recurse_ascending(n);
	std::cout << std::endl << std::endl;
}
