#include <cstdlib>
#include <iostream>

#include "../headers/arrays.hpp"


int generate_random_number(const int min, const int max);

void utl::array_random_fill(int arr[], const int size, const int min, const int max)
{
    for (int i = 0; i < size; ++i)
    {
        arr[i] = generate_random_number(min, max);
    }
}

void utl::print_arr(int arr[], const int size)
{
    for (int i = 0; i < size; ++i)
    {
        std::cout << arr[i] << " ";
    }
}

void utl::print_arr(int arr[], const int rows, const int columns)
{
    for (int i = 0; i < rows * columns; ++i)
    {
    	std::cout << arr[i] << " ";
        if((i + 1) % columns == 0)
        {
            std::cout << std::endl;
        }
    }
}


int generate_random_number(const int min, const int max)
{
    return min + std::rand() % (max - min + 1);
}
