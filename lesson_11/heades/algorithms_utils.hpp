#pragma once

enum class sorting_direction
{
    ascending,
    descending
};


bool find(const int* const arr, const int columns, const int rows, const int elem);

bool is_sorted(const int* const arr, const int size, const sorting_direction direction);

void walk_array(int** arr, const int columns, const int rows);
