#pragma once

#include <iostream>


namespace utl
{
    void array_random_fill(int arr[], const int size, const int min, const int max);

    void print_arr(const int arr[], const int size);

    void print_arr(int arr[], const int rows, const int columns);

    template<typename T>
    void print_arr(T arr, const int size)
    {
        print_arr(arr, static_cast<std::size_t>(size));
    }

    template<typename T>
    void print_arr(T arr, const std::size_t size)
    {
        for (std::size_t i = 0; i < static_cast<std::size_t>(size); ++i)
        {
            std::cout << arr[i] << " ";
        }
    }
}
