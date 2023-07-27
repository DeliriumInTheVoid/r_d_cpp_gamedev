#pragma once

enum class sorting_direction
{
    by_rows,
    by_column
};

void bubble_sort(int arr[], const int size);

void sort(int arr[][5], const int rows, const int columns, const sorting_direction direction);
