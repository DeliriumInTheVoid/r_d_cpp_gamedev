#pragma once

constexpr int array_size = 5;

enum class sorting_direction
{
    by_rows,
    by_column
};

void bubble_sort(int arr[], const int size);

void sort(int arr[][array_size], const int rows, const int columns, const sorting_direction direction);
