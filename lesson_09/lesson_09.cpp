#include <iostream>
#include <cctype>
#include <limits>

#include "headers/array_utils.hpp"

void print_menu();
void clear_console();

void translate_array();
void to_uppercase();
void palindrome();
void letter_types_count();
void compare_strings();

int main()
{
	std::cout << "Home Work 9\n";
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
			translate_array();
			break;
		case 2:
			to_uppercase();
			break;
		case 3:
			palindrome();
			break;
		case 4:
			letter_types_count();
			break;
		case 5:
			compare_strings();
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
	std::cout << "1. translate array\n";
	std::cout << "2. to uppercase\n";
	std::cout << "3. check palindrome\n";
	std::cout << "4. letter types count\n";
	std::cout << "5. compare strings\n";
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

void translate_array()
{
	constexpr int arr_size{ 10 };
	int arr[arr_size]{};

	std::cout << "Enter array of " << arr_size << " numbers:" << std::endl;

	for(int i = 0; i < arr_size; ++i)
	{
		int num{ 0 };
		std::cin >> num;
		arr[i] = num;
	}

	translate_array(arr, arr_size);

	std::cout << "translated array: \n " << std::endl;

	for (int i : arr)
	{
		std::cout << i << " ";
	}

	std::cout << std::endl;
}

void to_uppercase()
{
	constexpr int max_length = 101;
	char str[max_length];

	std::cout << "Enter line (max 100 chars):\n";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.getline(str, max_length);

	to_uppercase(str);

	std::cout << "Line in uppercase: " << str << std::endl;
}

void palindrome()
{
	constexpr int max_length = 101;
	char str[max_length];

	std::cout << "Enter line (max 100 chars):\n";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.getline(str, max_length);

	if (is_palindrome(str))
	{
		std::cout << "It\'s palindrome" << std::endl;
	}
	else
	{
		std::cout << "It\'s not palindrome" << std::endl;
	}
}


void letter_types_count()
{
	constexpr int max_length = 101;
	char str[max_length];

	int vowels_count{ 0 }, consonants_count{ 0 };

	std::cout << "Enter line (max 100 chars):\n";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.getline(str, max_length);

	parse_string_letters(str, vowels_count, consonants_count);

	std::cout << "vowels count: " << vowels_count << std::endl;
	std::cout << "consonants count: " << consonants_count << std::endl;

}

void compare_strings()
{
	constexpr int max_length = 101;
	char str1[max_length];
	char str2[max_length];

	std::cout << "Enter first string (max 100 chars):\n";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.getline(str1, max_length);

	std::cout << "Enter second string (max 100 chars):\n";
	std::cin.getline(str2, max_length);

	if (is_equal(str1, str2)) {
		std::cout << "strings are equal" << std::endl;
	}
	else {
		std::cout << "strings are not equal" << std::endl;
	}
}
