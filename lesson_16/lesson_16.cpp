#include <iostream>

#include "console.hpp"

#include "headers/rectangle.hpp"
#include "headers/vector2d.hpp"

void print_menu();

void rectangle_instances_counter();
void vector2d_operators();

int main()
{
    std::cout << "Home Work 16\n";
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
            rectangle_instances_counter();
            break;
        case 2:
            vector2d_operators();
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
    std::cout << "1. rectangle instances counter\n";
    std::cout << "2. class vector2d operators\n";
    std::cout << "3. Exit\n\n";

    std::cout << "Enter number:";
}

void rectangle_instances_counter()
{
    std::cout << "create rectangle" << std::endl;
    const rectangle rec{10.0, 2.0};
    std::cout << "rectangle instances count: " << rectangle::get_instances_count();
    std::cout << std::endl << std::endl;

    {
        std::cout << "create 2 rectangles" << std::endl;
        const rectangle rec1{ 10.0, 2.0 };
        const rectangle rec2{ 10.0, 2.0 };
        std::cout << "rectangle instances count: " << rectangle::get_instances_count();
        std::cout << std::endl << std::endl;
    }

    std::cout << "delete 2 rectangles" << std::endl;
    std::cout << "rectangle instances count: " << rectangle::get_instances_count();
    std::cout << std::endl << std::endl;

    std::cout << "create rectangle" << std::endl;
    const rectangle rec1{ 10.0, 2.0 };
    std::cout << "rectangle instances count: " << rectangle::get_instances_count();
    std::cout << std::endl << std::endl;
}

void vector2d_operators()
{
    vector2d v1{};
    vector2d v2{};

    std::cout << "enter x and y for first vector:\n";
    std::cin >> v1;
    std::cout << "Your vector: " << v1[0] << ", " << v1[1];

    std::cout << std::endl << std::endl;

    std::cout << "enter x and y for second vector:\n";
    std::cin >> v2;
    std::cout << "Your vector: " << v2;

    std::cout << std::endl << std::endl;

    std::cout << "v1 + v2: " << v1 + v2 << std::endl;
    std::cout << "v1 - v2: " << v1 - v2 << std::endl;
    vector2d v3{1,1};
    v3 = v2;
    std::cout << "v3 = v2; v3: " << v3 << std::endl;
    std::cout << "v3(): " << v3() << std::endl;
}
