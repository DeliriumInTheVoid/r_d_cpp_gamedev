#include <iostream>

#include "console.hpp"

#include "string_utils.hpp"


void print_menu();

void count_unique_words();
void find_most_occurred_word();
void validate_brackets();

int main()
{
    std::cout << "Home Work 22\n";
    std::cout << "-----------\n\n";

    int user_choice{ 0 };
    while (user_choice != 4)
    {
        print_menu();
        std::cin >> user_choice;
        utl::clear_console();
        switch (user_choice)
        {
        case 1:
            count_unique_words();
            break;
        case 2:
            find_most_occurred_word();
            break;
        case 3:
            validate_brackets();
            break;
        case 4:
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
    std::cout << "1. count unique words\n";
    std::cout << "2. find most occurred word\n";
    std::cout << "3. validate brackets\n";
    std::cout << "4. Exit\n\n";

    std::cout << "Enter number:";
}

void count_unique_words()
{
    const std::string line = "Hello Hey Hello Bye Hey";
    std::cout << "string is: " << line << std::endl;

    const unsigned int count = unique_words_count(line);
    std::cout << "unique words: " << count << std::endl;
}

void find_most_occurred_word()
{
    const std::string line = "Hello Hey Hello Bye Hey Hey";
    std::cout << "string is: " << line << std::endl;

    const std::string most_common = most_occurred_word(line);
    std::cout << "most common word: " << most_common << std::endl;
}

void validate_brackets()
{
    const std::string balanced_str = "{ [()][ ] }()";
    const std::string unbalanced_str = "[{]}";

    if (is_brackets_balanced(balanced_str))
    {
        std::cout << "brackets balanced: " << balanced_str << std::endl;
    }
    else
    {
        std::cout << "brackets unbalanced: " << balanced_str << std::endl;
    }

    if (is_brackets_balanced(unbalanced_str))
    {
        std::cout << "brackets balanced: " << unbalanced_str << std::endl;
    }
    else
    {
        std::cout << "brackets unbalanced: " << unbalanced_str << std::endl;
    }
}
