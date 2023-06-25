#include <iostream>
#include <iomanip>


void print_menu();
void clear_console();
void swap_numbers();
void double_manipulators();
void double_format();
void print_enum();
void bool_manipulators();


enum class week_day
{
	monday = 1,
	tuesday,
	wednesday,
	thursday,
	friday,
	saturday,
	sunday
};

int main()
{
	std::cout << "Home Work 3\n";
	std::cout << "-----------\n\n";

	int user_choice{ 0 };
	while (user_choice != 6)
	{
		print_menu();
		std::cin >> user_choice;
		clear_console();
		switch (user_choice)
		{
		case 1:
			swap_numbers();
			break;
		case 2:
			double_manipulators();
			break;
		case 3:
			double_format();
			break;
		case 4:
			print_enum();
			break;
		case 5:
			bool_manipulators();
			break;
		case 6:
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
	std::cout << "1. Swap numbers\n";
	std::cout << "2. \"double\" manipulators\n";
	std::cout << "3. \"double\" format\n";
	std::cout << "4. \"enum\" print\n";
	std::cout << "5. \"bool\" manipulators\n";
	std::cout << "6. Exit\n\n";

	std::cout << "Enter number:";
}

void clear_console() {
#ifdef _WIN32
	std::system("cls");
#else
	std::cout << "\033[2J\033[1;1H";
#endif
}

void swap_numbers()
{
	int a{ 0 };
	int b{ 0 };
	std::cout << "Enter integer 1:\n";
	std::cin >> a;
	std::cout << "Enter integer 2:\n";
	std::cin >> b;

	const int temp{ a };
	a = b;
	b = temp;

	std::cout << "Integer 1: " << a << "\n";
	std::cout << "Integer 2: " << b << "\n";
}

void double_manipulators()
{
	double value{ 3.14159 };
	std::cout << "double value: " << value << std::endl;

	int int_value = static_cast<int>(value);
	std::cout << "cast double value to int: " << int_value << std::endl;

	std::cout << "double in a row with manips: "
		<< std::setw(20) << std::right << std::setfill('$') << value
		<< std::endl;
}

void double_format()
{
	double value{ 3.14159 };
	std::cout << "double value: " << value << std::endl;

	std::cout << "\nprecisions:" << std::endl;
	std::cout << "precision 2: " << std::fixed << std::setprecision(2) << value << std::endl;
	std::cout << "precision 5: " << std::fixed << std::setprecision(5) << value << std::endl;
	std::cout << "precision 10: " << std::fixed << std::setprecision(10) << value << std::endl;

	std::cout << "\nformats:" << std::endl;
	std::cout << "default format: " << std::defaultfloat << value << std::endl;
	std::cout << "scientific format: " << std::scientific << value << std::endl;
	std::cout << "hexfloat format: " << std::hexfloat << value << std::endl;
	std::cout << "fixed format: " << std::fixed << value << std::endl;
}

void print_enum()
{
	std::cout << "int values of week_day enum:" << std::endl;
	for (int day = static_cast<int>(week_day::monday); day <= static_cast<int>(week_day::sunday); day++)
	{
		std::cout << day << std::endl;
	}
}

void bool_manipulators()
{
	int value;
	std::cout << "Enter number: ";
	std::cin >> value;

	bool boolean_value = static_cast<bool>(value);

	std::cout << "value in bool format: " << std::boolalpha << boolean_value << std::endl;
}