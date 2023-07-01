#include <iostream>
#include <limits>


void print_menu();
void clear_console();

void max_from_three_numbers();
void min_from_two_numbers();
void division_to_5_and_11();
void triangle_from_angles();
void month_num_to_name();
void month_num_to_season();

enum class month {
	january = 1,
	february,
	march,
	april,
	may,
	june,
	july,
	august,
	september,
	october,
	november,
	december
};

int main()
{
	std::cout << "Home Work 5\n";
	std::cout << "-----------\n\n";

	int user_choice{ 0 };
	while (user_choice != 7)
	{
		print_menu();
		std::cin >> user_choice;
		clear_console();
		switch (user_choice)
		{
		case 1:
			max_from_three_numbers();
			break;
		case 2:
			min_from_two_numbers();
			break;
		case 3:
			division_to_5_and_11();
			break;
		case 4:
			triangle_from_angles();
			break;
		case 5:
			month_num_to_name();
			break;
		case 6:
			month_num_to_season();
			break;
		case 7:
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
	std::cout << "1. max from three numbers\n";
	std::cout << "2. min from two numbers\n";
	std::cout << "3. division to 5 and 11\n";
	std::cout << "4. triangle from angles\n";
	std::cout << "5. month num to name\n";
	std::cout << "6. month num to season\n";
	std::cout << "7. Exit\n\n";

	std::cout << "Enter number:";
}

void clear_console() {
#ifdef _WIN32
	std::system("cls");
#else
	std::cout << "\033[2J\033[1;1H";
#endif
}

void max_from_three_numbers()
{
	double a{ 0 }, b{ 0 }, c{ 0 }, max{ std::numeric_limits< double >::lowest() };

	std::cout << "Enter number 1:\n";
	std::cin >> a;
	if (a > max)
	{
		max = a;
	}

	std::cout << "Enter number 2:\n";
	std::cin >> b;
	if (b > max)
	{
		max = b;
	}

	std::cout << "Enter number 3:\n";
	std::cin >> c;
	if (c > max)
	{
		max = c;
	}

	std::cout << std::endl;

	std::cout << "max from [" <<a <<", " << b << ", " << c <<"] is " << max << std::endl;
}

void min_from_two_numbers()
{
	double a{ 0 }, b{ 0 };

	std::cout << "Enter number 1:\n";
	std::cin >> a;

	std::cout << "Enter number 2:\n";
	std::cin >> b;

	std::cout << std::endl;

	std::cout << "min from [" << a << ", " << b << "] is " << (a < b ? a : b) << std::endl;
}

void division_to_5_and_11()
{
	int number{ 0 };

	std::cout << "Enter number:\n";
	std::cin >> number;

	if (number % 5 == 0 && number % 11 == 0) {
		std::cout << "number " << number <<" is div to 5 and 11" << std::endl;
	}
	else {
		std::cout << "number " << number << " is not div to 5 and 11" << std::endl;
	}
}

void triangle_from_angles()
{
	double angle1{ 0 }, angle2{ 0 }, angle3{ 0 };

	std::cout << "Enter angle 1:\n";
	std::cin >> angle1;

	std::cout << "Enter angle 2:\n";
	std::cin >> angle2;

	std::cout << "Enter angle 3:\n";
	std::cin >> angle3;

	double diff{ 180 - angle1 - angle2 - angle3 };
	diff = diff < 0 ? -diff : diff;

	if (diff <= std::numeric_limits< double >::epsilon()) {
		std::cout << "angles [" << angle1 << ", " << angle2 << ", " << angle3 << "] form a triangle" << std::endl;
	}
	else {
		std::cout << "angles [" << angle1 << ", " << angle2 << ", " << angle3 << "] do not form a triangle" << std::endl;
	}
}

void month_num_to_name()
{
	int month_number{ 0 };

	std::cout << "Enter month number:\n";
	std::cin >> month_number;

	const auto month_value = static_cast<month>(month_number);

	switch (month_value)
	{
	case month::january:
		std::cout << "month number " << month_number << " is January";
		break;
	case month::february:
		std::cout << "month number " << month_number << " is February";
		break;
	case month::march:
		std::cout << "month number " << month_number << " is March";
		break;
	case month::april:
		std::cout << "month number " << month_number << " is April";
		break;
	case month::may:
		std::cout << "month number " << month_number << " is May";
		break;
	case month::june:
		std::cout << "month number " << month_number << " is June";
		break;
	case month::july:
		std::cout << "month number " << month_number << " is July";
		break;
	case month::august:
		std::cout << "month number " << month_number << " is August";
		break;
	case month::september:
		std::cout << "month number " << month_number << " is September";
		break;
	case month::october:
		std::cout << "month number " << month_number << " is October";
		break;
	case month::november:
		std::cout << "month number " << month_number << " is November";
		break;
	case month::december:
		std::cout << "month number " << month_number << " is December";
		break;
	default:
		std::cout << "Wrong month number " << month_number << ". Must be 1..12.";
		break;
	}
}

void month_num_to_season()
{
	int month_number{ 0 };

	std::cout << "Enter month number:\n";
	std::cin >> month_number;

	const auto month_value = static_cast<month>(month_number);

	switch (month_value)
	{
	case month::january:
	case month::february:
	case month::december:
		std::cout << "month number " << month_number << " is Winter";
		break;
	case month::march:
	case month::april:
	case month::may:
		std::cout << "month number " << month_number << " is Spring";
		break;
	case month::june:
	case month::july:
	case month::august:
		std::cout << "month number " << month_number << " is Summer";
		break;
	case month::september:
	case month::october:
	case month::november:
		std::cout << "month number " << month_number << " is Autumn";
		break;
	default:
		std::cout << "Wrong month number " << month_number << ". Must be 1..12.";
		break;
	}
}
