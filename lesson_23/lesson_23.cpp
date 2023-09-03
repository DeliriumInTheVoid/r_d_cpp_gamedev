#include <iostream>

#include "console.hpp"
#include "arrays.hpp"

#include "dynamic_array.hpp"
#include "inplace_array.h"


void print_menu();

void dynamic_array_use_cases();
void inplace_array_use_cases();


int main()
{
    std::cout << "Home Work 23\n";
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
            dynamic_array_use_cases();
            break;
        case 2:
            inplace_array_use_cases();
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
    std::cout << "1. dynamic array use cases\n";
    std::cout << "2. inplace array use cases\n";
    std::cout << "3. Exit\n\n";

    std::cout << "Enter number:";
}

void dynamic_array_use_cases()
{
    std::cout << "Create array with size:" << std::endl;
    dynamic_array<int> arr{ 5 };
    utl::print_arr(arr, arr.get_size());
    std::cout << std::endl;

    std::cout << "Resize array:" << std::endl;
    arr[0] = 1;
    arr[1] = 2;
    arr.set_size(10);
    utl::print_arr(arr, arr.get_size());
    std::cout << std::endl;
    arr.set_size(5);
    utl::print_arr(arr, arr.get_size());
    std::cout << std::endl;

    std::cout << "Extend array:" << std::endl;
    arr.push_back(6);
    arr.push_back(7);
    arr.push_back(8);
    utl::print_arr(arr, arr.get_size());
    std::cout << std::endl;

    std::cout << "Copy constructor for array:" << std::endl;
    dynamic_array arr_copy{ arr };
    arr.push_back(9);
    utl::print_arr(arr_copy, arr_copy.get_size());
    std::cout << std::endl;

    std::cout << "Copy assignment for array:" << std::endl;
    dynamic_array<int> arr_copy_assignment{};
    arr_copy_assignment.push_back(100);
    arr_copy_assignment = arr;
    utl::print_arr(arr_copy_assignment, arr_copy_assignment.get_size());
    std::cout << std::endl;

    std::cout << "clear() array and push_back number:" << std::endl;
    arr.clear();
    arr.push_back(0);
    utl::print_arr(arr, arr.get_size());
    std::cout << std::endl;
}


void inplace_array_use_cases()
{
    std::cout << "Create array with size:" << std::endl;
    inplace_array<int, 5> arr{ 5 };
    utl::print_arr(arr, arr.get_size());
    std::cout << std::endl;

    std::cout << "Resize array:" << std::endl;
    arr[0] = 1;
    arr[1] = 2;
    arr.set_size(10);
    utl::print_arr(arr, arr.get_size());
    std::cout << std::endl;
    arr.set_size(5);
    utl::print_arr(arr, arr.get_size());
    std::cout << std::endl;

    std::cout << "Extend array:" << std::endl;
    arr.push_back(6);
    arr.push_back(7);
    arr.push_back(8);
    utl::print_arr(arr, arr.get_size());
    std::cout << std::endl;

    std::cout << "Copy constructor for array:" << std::endl;
    inplace_array arr_copy{ arr };
    arr.push_back(9);
    utl::print_arr(arr_copy, arr_copy.get_size());
    std::cout << std::endl;

    std::cout << "Copy assignment for array:" << std::endl;
    inplace_array<int, 5> arr_copy_assignment{};
    arr_copy_assignment.push_back(100);
    arr_copy_assignment = arr;
    utl::print_arr(arr_copy_assignment, arr_copy_assignment.get_size());
    std::cout << std::endl;

    std::cout << "clear() array and push_back number:" << std::endl;
    arr.clear();
    arr.push_back(0);
    utl::print_arr(arr, arr.get_size());
    std::cout << std::endl;
}
