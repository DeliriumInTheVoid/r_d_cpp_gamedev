#include <iostream>

#include "vld.h"
#include "console.hpp"

#include "headers/dynamic_int_array.hpp"
#include "headers/custom_string.hpp"


void print_menu();
void print_arr(dynamic_int_array& arr);

void dynamic_int_array_use_cases();
void detect_memory_leaks();


int main()
{
    std::cout << "Home Work 17\n";
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
            dynamic_int_array_use_cases();
            break;
        case 2:
            detect_memory_leaks();
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
    std::cout << "1. dynamic int array use cases\n";
    std::cout << "2. detect memory leaks\n";
    std::cout << "3. Exit\n\n";

    std::cout << "Enter number:";
}

void dynamic_int_array_use_cases()
{
    dynamic_int_array arr{ 5 };

    arr.push_back(1);
    arr.push_back(2);
    arr.push_back(3);
    arr.push_back(4);
    arr.push_back(5);
    std::cout << "Create array:" << std::endl;
    print_arr(arr);
    std::cout << std::endl;

    arr.push_back(6);
    arr.push_back(7);
    arr.push_back(8);
    std::cout << "Extend array:" << std::endl;
    print_arr(arr);
    std::cout << std::endl;

    dynamic_int_array arr_copy{ arr };
    arr.push_back(9);
    std::cout << "Copy constructor for array:" << std::endl;
    print_arr(arr_copy);
    std::cout << std::endl;

    dynamic_int_array arr_copy_assignment{};
    arr_copy_assignment.push_back(100);
    arr_copy_assignment = arr;
    std::cout << "Copy assignment for array:" << std::endl;
    print_arr(arr_copy_assignment);
    std::cout << std::endl;

    std::cout << "clear() array and push_back number:" << std::endl;
    arr.clear();
    arr.push_back(0);
    print_arr(arr);
    std::cout << std::endl;
}


void detect_memory_leaks()
{
    //CASE 1
    StringCustom a;
    a = "Hello";
    a = "There";

    //CASE 2 ??
    StringCustom b;
    b.assign("Hello Here");
    b.assign("Simple hello");

    //CASE 3
    //Suppose it was read dynamically from somewhere
    int ArraySize = 10;
    int* arrayInt = allocateArray(ArraySize);
    clearArray(arrayInt);

    //CASE 4
    int rows = 10;
    int columns = 20;
    int** array = allocateArrayOfArrays(rows, columns);
    clearArrayOfArrays(array, 10);
}

void print_arr(dynamic_int_array& arr)
{
    const std::size_t size{ arr.get_size() };
    for (std::size_t i = 0; i < size; ++i)
    {
        std::cout << arr[i] << " ";
    }
}
