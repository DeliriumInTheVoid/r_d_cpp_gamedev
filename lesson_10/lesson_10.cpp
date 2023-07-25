#include <iostream>

#include "headers/pointers_utils.hpp"

void print_menu();
void clear_console();

void swap_numbers();
void calculate_sum();
void find_number();

int main()
{
    std::cout << "Home Work 10\n";
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
            swap_numbers();
            break;
        case 2:
            calculate_sum();
            break;
        case 3:
            find_number();
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


void print_menu()
{
    std::cout << "1. swap numbers\n";
    std::cout << "2. calculate sum\n";
    std::cout << "3. find number\n";
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

void swap_numbers()
{
    int a{ 0 }, b{ 0 };

    std::cout << "Enter two numbers to swap:\n";

    std::cin >> a >> b;

    int tmp_a{ a };
    int tmp_b{ b };

    swap(tmp_a, tmp_b);
    std::cout << "swap with ref: a = " << tmp_a << ", b = " << tmp_b << std::endl;

    tmp_a = a;
    tmp_b = b;

    swap(&tmp_a, &tmp_b);
    std::cout << "swap with pointers: a = " << tmp_a << ", b = " << tmp_b << std::endl;
}

void calculate_sum()
{
    constexpr int size{ 5 };

    constexpr double arr[size] = { 1.5, 2.0, 3.7, 4.2, 5.9 };

    double sum = { 0.0 };

    if (calculate_sum(arr, size, sum))
    {
        std::cout << "array elements sum: " << sum << std::endl;
    }
    else
    {
        std::cout << "incorrect array size" << std::endl;
    }
}

void find_number()
{
    constexpr int size{ 8 };
    constexpr int arr[size] = { 1, 2, 3, 4, 5, 6, 7, 8 };

    int elem{ 0 };

    std::cout << "enter number to check: ";
    std::cin >> elem;

    if (find(arr, size, elem)) {
        std::cout << "number " << elem << " is in array" << std::endl;
    }
    else {
        std::cout << "number " << elem << " is not in array" << std::endl;
    }
}
