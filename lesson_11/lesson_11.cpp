#include <iostream>
#include <iomanip>

#include "heades/algorithms_utils.hpp"

void print_menu();
void clear_console();

void find_number();
void is_sorted();
void walk_array();

int main()
{
    std::cout << "Home Work 11\n";
    std::cout << "-----------\n\n";

    int user_choice{ 0 };
    while (user_choice != 4)
    {
        print_menu();
        std::cin >> user_choice;
        clear_console();
        switch (user_choice)
        {
        case 1:
            find_number();
            break;
        case 2:
            is_sorted();
            break;
        case 3:
            walk_array();
            break;
        case 4:
            std::cout << "Exit\n";
            break;
        default:
            std::cout << "Wrong choice!";
            user_choice = 0;
            break;
        }
        std::cout << "\nPress Enter to continue" << std::flush;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
        clear_console();
    }

    return 0;
}


void print_menu()
{
    std::cout << "1. find number\n";
    std::cout << "2. is sorted\n";
    std::cout << "3. walk array\n";
    std::cout << "4. Exit\n\n";

    std::cout << "Enter number:";
}

void clear_console() {
#ifdef _WIN32
    std::system("cls");
#else
    std::cout << "\033[2J\033[1;1H";
#endif
}

void find_number()
{
    constexpr int col_num{ 4 };
    constexpr int row_num{ 3 };
    constexpr int arr[row_num][col_num]{ {1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12 } };

    int elem{ 0 };

    std::cout << "enter number to check: ";
    std::cin >> elem;


    if (find(&arr[0][0], col_num, row_num, elem)) {
        std::cout << "number " << elem << " is in array" << std::endl;
    }
    else {
        std::cout << "number " << elem << " is not in array" << std::endl;
    }
}


void is_sorted()
{
    constexpr int size{ 5 };

    constexpr int arr_asc[size]{ 1, 2, 3, 4, 5, };
    constexpr int arr_desc[size]{ 5, 4, 3, 2, 1, };

    std::cout << "arr_asc is ascending: " << std::boolalpha << is_sorted(arr_asc, size, sorting_direction::ascending) << std::endl;
    std::cout << "arr_asc is descending: " << is_sorted(arr_asc, size, sorting_direction::descending) << std::endl;

    std::cout << "arr_desc is ascending: " << is_sorted(arr_desc, size, sorting_direction::ascending) << std::endl;
    std::cout << "arr_desc is descending: " << is_sorted(arr_desc, size, sorting_direction::descending) << std::endl;
}

void walk_array()
{
    constexpr int columns_num{ 4 };
    constexpr int rows_num{ 3 };

    int** arr = new int* [rows_num];

    for (int i = 0; i < rows_num; i++) {
        arr[i] = new int[columns_num];
        for (int j = 0; j < columns_num; j++) {
            arr[i][j] = i * columns_num + j + 1;
        }
    }

    walk_array(arr, columns_num, rows_num);

    for (int i = 0; i < rows_num; ++i) {
        delete[] arr[i];
    }
    delete[] arr;
}
