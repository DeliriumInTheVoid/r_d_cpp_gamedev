#include <iostream>

#include "../heades/algorithms_utils.hpp"

bool find(const int* const arr, const int columns, const int rows, const int elem)
{
    const int size{ columns * rows };

    for (int i = 0; i < size; ++i)
    {
        if (*(arr + i) == elem)
        {
            return true;
        }
    }

    return false;
}

bool is_sorted(const int* const arr, const int size, const sorting_direction direction)
{
    for (int i = 0; i < size - 1; ++i)
    {
        if (direction == sorting_direction::ascending)
        {
            if (arr[i] > arr[i + 1])
            {
                return false;
            }
        }
        else
        {
            if (arr[i] < arr[i + 1])
            {
                return false;
            }
        }
    }

    return true;
}

void walk_array(int** arr, const int columns, const int rows)
{
    for (int j = columns - 1; j >= 0; --j)
    {
        for (int i = 0; i < rows; ++i)
        {
            std::cout << arr[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << std::endl << std::endl;

    for (int i = rows - 1; i >= 0; --i)
    {
        if (i % 2 == 0)
        {
            for (int j = 0; j < columns; ++j)
            {
                std::cout << arr[i][j] << " ";
            }
        }
        else
        {
            for (int j = columns - 1; j >= 0; --j)
            {
                std::cout << arr[i][j] << " ";
            }
        }
        std::cout << "\n";
    }

}
