#include <iostream>
#include <iomanip>

#include "arrays.hpp"
#include "console.hpp"

#include "headers/sort_utils.hpp"

void print_menu();

void bubble_sort();
void q_sort();


int main()
{
    std::srand(std::time(nullptr));
    std::cout << "Home Work 12\n";
    std::cout << "-----------\n\n";

    int user_choice{ 0 };
    while (user_choice != 3)
    {
        print_menu();
        std::cin >> user_choice;
        utl::clear_console();
        switch (user_choice)
        {
        case 1:
            bubble_sort();
            break;
        case 2:
            q_sort();
            break;
        case 3:
            std::cout << "Exit\n";
            break;
        default:
            std::cout << "Wrong choice!";
            user_choice = 0;
            break;
        }
        utl::wait_to_continue();
    }

    return 0;
}


void print_menu()
{
    std::cout << "1. bubble sort\n";
    std::cout << "2. quick sort\n";
    std::cout << "3. Exit\n\n";

    std::cout << "Enter number:";
}

void bubble_sort()
{
    std::cout << "enter array size for generating: \n";

    int size{ 0 };
    std::cin >> size;

    int* arr = new int[size];
    utl::array_random_fill(arr, size, -10, 10);

    std::cout << "generated array:\n";
    utl::print_arr(arr, size);

    std::cout << std::endl;

    bubble_sort(arr, size);
    std::cout << "sorted array:\n";
    utl::print_arr(arr, size);

    delete[] arr;
}


void q_sort()
{
    constexpr int rows{ 4 };
    constexpr int columns{ 5 };

    constexpr int arr_size = rows * columns;

    int arr[rows][columns]{};

    utl::array_random_fill(arr[0], arr_size, 0, 9);
    std::cout << "array before sorting:\n";
    utl::print_arr(arr[0], rows, columns);

    sort(arr, rows, columns, sorting_direction::by_rows);

    std::cout << std::endl;
    std::cout << "array after sorting by rows:\n";
    utl::print_arr(arr[0], rows, columns);

    std::cout << std::endl << std::endl;

    utl::array_random_fill(arr[0], arr_size, 0, 9);
    std::cout << "array before sorting:\n";
    utl::print_arr(arr[0], rows, columns);

    sort(arr, rows, columns, sorting_direction::by_column);

    std::cout << std::endl;
    std::cout << "array after sorting by column:\n";
    utl::print_arr(arr[0], rows, columns);
}
