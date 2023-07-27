#include "../headers/sort_utils.hpp"

void quick_sort_in_row(int arr[][5], const int low, const int high, const int row);
int get_pivot_index_in_row(int arr[][5], const int low, const int high, const int row);

void quick_sort_in_column(int arr[][5], const int low, const int high, const int column);
int get_pivot_index_in_column(int arr[][5], const int low, const int high, const int column);

void swap(int& a, int& b);


void bubble_sort(int arr[], const int size)
{
    for (int i = 0; i < size - 1; ++i)
    {
        bool had_swap{ false };
        for (int j = 0; j < size - 1 - i; ++j)
        {
            if (arr[j] > arr[j + 1])
            {
                const int tmp{ arr[j] };
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
                had_swap = true;
            }
        }

        if (!had_swap)
        {
            break;
        }
    }
}

void sort(int arr[][5], const int rows, const int columns, const sorting_direction direction)
{
    if (direction == sorting_direction::by_column)
    {
        for (int i = 0; i < columns; ++i)
        {
            quick_sort_in_column(arr, 0, rows - 1, i);
        }
    }
	else if (direction == sorting_direction::by_rows)
    {
        for (int i = 0; i < rows; ++i)
        {
            quick_sort_in_row(arr, 0, columns - 1, i);
        }
    }

}

void quick_sort_in_row(int arr[][5], const int low, const int high, const int row)
{
    if (low < high)
    {
        const int pivot_index = get_pivot_index_in_row(arr, low, high, row);

        quick_sort_in_row(arr, low, pivot_index - 1, row);
        quick_sort_in_row(arr, pivot_index + 1, high, row);
    }
}

int get_pivot_index_in_row(int arr[][5], const int low, const int high, const int row)
{
    const int pivot = arr[row][high];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (arr[row][j] <= pivot)
        {
            i++;
            swap(arr[row][i], arr[row][j]);
        }
    }

    swap(arr[row][i + 1], arr[row][high]);

    return i + 1;
}

void quick_sort_in_column(int arr[][5], const int low, const int high, const int column)
{
    if (low < high)
    {
        const int pivot_index = get_pivot_index_in_column(arr, low, high, column);

        quick_sort_in_column(arr, low, pivot_index - 1, column);
        quick_sort_in_column(arr, pivot_index + 1, high, column);
    }
}

int get_pivot_index_in_column(int arr[][5], const int low, const int high, const int column)
{
    const int pivot = arr[high][column];
    int i = low - 1;

    for (int j = low; j < high; j++)
    {
        if (arr[j][column] <= pivot)
        {
            i++;
            swap(arr[i][column], arr[j][column]);
        }
    }

    swap(arr[i + 1][column], arr[high][column]);

    return i + 1;
}

void swap(int& a, int& b)
{
    const int tmp = a;
    a = b;
    b = tmp;
}
