#include "../headers/pointers_utils.hpp"

void swap(int& a, int& b)
{
    const int temp = a;
    a = b;
    b = temp;
}

void swap(int* a, int* b)
{
    const int temp = *a;
    *a = *b;
    *b = temp;
}

bool calculate_sum(const double* const arr, const int arr_size, double& sum)
{
    if (arr_size <= 0)
    {
        return false;
    }

    sum = 0.0;

    for (int i = 0; i < arr_size; i++)
    {
        sum += arr[i];
    }

    return true;
}

bool find(const int* const arr, const int size, const int elem)
{
    for (int i = 0; i < size; ++i)
    {
        if (*(arr + i) == elem)
        {
            return true;
        }
    }

    return false;
}
