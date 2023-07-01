#include <iostream>

void print_menu();
void clear_console();

void arithmetic_progression();
void fibonacci();
void factorial();
void draw_shapes();
void char_calculations();

int main()
{
	std::cout << "Home Work 6\n";
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
			arithmetic_progression();
			break;
		case 2:
			fibonacci();
			break;
		case 3:
			factorial();
			break;
		case 4:
			draw_shapes();
			break;
		case 5:
			char_calculations();
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
	std::cout << "1. arithmetic progression\n";
	std::cout << "2. fibonacci\n";
	std::cout << "3. factorial\n";
	std::cout << "4. draw shapes\n";
	std::cout << "5. char calculations\n";
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

void arithmetic_progression()
{
	double first_element{ 0 }, difference{ 0 };
	int number_of_elements{ 0 };

	std::cout << "enter first element:\n";
	std::cin >> first_element;

	std::cout << "enter difference:\n";
	std::cin >> difference;

	std::cout << "enter number of elements:\n";
	std::cin >> number_of_elements;
	if (number_of_elements < 1)
	{
		std::cout << "number of elements must be > 0";
		return;
	}

	std::cout << std::endl;

	double current_element = first_element;
	for (int i = 0; i < number_of_elements; i++)
	{
		std::cout << current_element << "; ";
		current_element += difference;
	}
}

void fibonacci()
{
	int n{ 0 };
	long long result{ 0 };

	std::cout << "Enter fibonacci element num:\n";
	std::cin >> n;

	if (n <= 0) {
		result = 0;
	}
	else if (n == 1) {
		result = 1;
	}
	else {
		long long prev = 0;
		result = 1;

		for (int i = 2; i <= n; i++) {
			const long long next = prev + result;
			prev = result;
			result = next;
		}
	}

	std::cout << "fibonacci value for element " << n << " is " << result;
}

void factorial()
{
	int n{ 0 };
	long long result{ 0 };

	std::cout << "Enter number:\n";
	std::cin >> n;

	if (n < 0 || n > 25) {
		std::cout << "number must be 0..25" << std::endl;
		return;
	}
	
	if (n == 0 || n == 1) {
		result = 1;
	}
	else {
		long long current_value = 1;

		for (int i = 2; i <= n; i++) {
			current_value *= i;
		}

		result = current_value;
	}

	std::cout << "factorial value for number " << n << " is " << result;
}


void draw_shapes()
{
	int rows{ 0 };

	std::cout << "Enter row num:\n";
	std::cin >> rows;

	if (rows < 1)
	{
		std::cout << "row num must be > 0\n";
		return;
	}

	int elements{ 0 };
	std::cout << "a. upside down triangle:\n";
	for (int i = 0; i < rows; ++i)
	{
		elements = rows - i;
		while (--elements >= 0)
		{
			std::cout << "*";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	std::cout << "b. triangle:\n";
	for (int i = 0; i < rows; ++i)
	{
		elements = i + 1;
		while (--elements >= 0)
		{
			std::cout << "*";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	std::cout << "c. square:\n";
	for (int i = 0; i < rows; ++i)
	{
		elements = rows;
		while (--elements >= 0)
		{
			std::cout << "*";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	std::cout << "d. parallelogram:\n";
	for (int i = 0; i < rows; ++i)
	{
		elements = i;
		while (--elements >= 0)
		{
			std::cout << " ";
		}
		elements = rows;
		while (--elements >= 0)
		{
			std::cout << "*";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	std::cout << "e. triangle with (1, 0) symbols:\n";
	for (int i = 0; i < rows; ++i)
	{
		elements = i + 1;
		while (--elements >= 0)
		{
			std::cout << (elements % 2 == 0 ? 1 : 0);
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void char_calculations()
{
	char input{ '\0' };
	int sum{ 0 };

	std::cout << "enter char '.' to stop.\n";

	while (true) {
		std::cin >> input;

		if (input == '.') {
			std::cout << "stop\n" << std::endl;
			break;
		}

		if (input >= 'a' && input <= 'z') {
			char uppercase = input - 'a' + 'A';
			std::cout << "uppercase value is: " << uppercase << std::endl;
		}
		else if (input >= '0' && input <= '9') {
			const int digit = input - '0';
			sum += digit;
			std::cout << "sum: " << sum << std::endl;
		}
		else {
			std::cout << "this '" << input << "' char does not process. please enter another one" << std::endl;
		}
	}
}
