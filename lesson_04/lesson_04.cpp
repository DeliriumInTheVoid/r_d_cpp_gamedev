#include <iostream>
#include <iomanip>
#include <numbers>

void print_menu();
void clear_console();

void sec_to_date_time();
void number_operators();
void compare_numbers();
void rectangle_calculations();
void circle_calculations();

int main()
{
	std::cout << "Home Work 4\n";
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
			sec_to_date_time();
			break;
		case 2:
			number_operators();
			break;
		case 3:
			compare_numbers();
			break;
		case 4:
			rectangle_calculations();
			break;
		case 5:
			circle_calculations();
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
	std::cout << "1. seconds to datetime\n";
	std::cout << "2. number operators\n";
	std::cout << "3. compare numbers\n";
	std::cout << "4. rectangle calculations\n";
	std::cout << "5. circle calculations\n";
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

void sec_to_date_time()
{
	int sec{ 0 };
	std::cout << "enter seconds:";
	std::cin >> sec;

	if(sec <= 0)
	{
		std::cout << "Wrong input!";
		return;
	}

	constexpr int sec_in_day{ 60 * 60 * 24 };
	int days = sec / sec_in_day;
	sec -= days * sec_in_day;

	constexpr int sec_in_hour{ 60 * 60 };
	int hours = sec / sec_in_hour;
	sec -= hours * sec_in_hour;

	constexpr int sec_in_minute{ 60 };
	int minutes = sec / sec_in_minute;
	sec -= minutes * sec_in_minute;

	int seconds = sec;

	if (days > 0)
	{
		std::cout << "days: " << days << std::endl;
	}
	if (hours > 0)
	{
		std::cout << "hours: " << hours << std::endl;
	}
	if (minutes > 0)
	{
		std::cout << "minutes: " << minutes << std::endl;
	}
	
	std::cout << "seconds: " << seconds << std::endl;
}

void number_operators()
{
	int number1, number2, number3;
	std::cout << "enter first number: ";
	std::cin >> number1;

	std::cout << "enter second number: ";
	std::cin >> number2;

	std::cout << "enter third number: ";
	std::cin >> number3;

	int sum = number1 + number2 + number3;
	int product = number1 * number2 * number3;
	double average = sum / 3.0;

	std::cout << std::endl;

	std::cout << "sum: " << sum << std::endl;
	std::cout << "product: " << product << std::endl;
	std::cout << "average: " << average << std::endl;

	std::cout << std::endl;

	if (number1 == number2)
	{
		std::cout << "number1 equals to number2";
	}
	else
	{
		if (number1 > number2)
			std::cout << "number 1 is greater than number 2";
		else
			std::cout << "number 1 is less than number 2";
	}

	std::cout << std::endl;

	if (number2 == number3)
	{
		std::cout << "number2 equals to number3";
	}
	else
	{
		if (number2 > number3)
			std::cout << "number 2 is greater than number 3";
		else
			std::cout << "number 2 is less than number 3";
	}

	std::cout << std::endl;
}

void compare_numbers()
{
	int number1, number2;

	std::cout << "enter first number: ";
	std::cin >> number1;

	std::cout << "enter second number: ";
	std::cin >> number2;

	bool less_than = number1 < number2;
	bool equal = number1 == number2;
	bool more_than = number1 > number2;
	bool less_or_equal = number1 <= number2;

	std::cout << std::endl;

	std::cout << "boolalpha formatted results" << std::endl;
	std::cout << "number1 < number2: " << std::boolalpha << less_than << std::endl;
	std::cout << "number1 == number2: " << equal << std::endl;
	std::cout << "number1 > number2: " << more_than << std::endl;
	std::cout << "number1 <= number2: " << less_or_equal << std::endl;

	std::cout << std::endl;

	std::cout << "noboolalpha formatted results:" << std::endl;
	std::cout << "number1 < number2: " << std::noboolalpha << less_than << std::endl;
	std::cout << "number1 == number2: " << equal << std::endl;
	std::cout << "number1 > number2: " << more_than << std::endl;
	std::cout << "number1 <= number2: " << less_or_equal << std::endl;
}


void rectangle_calculations()
{
	double width, height;

	std::cout << "enter width: ";
	std::cin >> width;

	std::cout << "enter height: ";
	std::cin >> height;

	double area = width * height;
	double perimeter = 2 * (width + height);

	std::cout << std::endl;

	std::cout << "area: " << area << std::endl;
	std::cout << "perimeter: " << perimeter << std::endl;
}

void circle_calculations()
{
	double radius;

	std::cout << "enter circle radius: ";
	std::cin >> radius;

	double area = std::numbers::pi * radius * radius;
	double circumference = 2 * std::numbers::pi * radius;

	std::cout << std::endl;

	std::cout << "circle area: " << area << std::endl;
	std::cout << "circumference: " << circumference << std::endl;
}
