#include <algorithm>
#include <iostream>

#include "arrays.hpp"
#include "console.hpp"

#include "HWs.hpp"

void print_menu();
void print_vector_strings(const std::vector<std::string>& vec);

void check_lambda_issue();
void divide_by_number_in_vector();
void sort_strings_vec_by_length();

int main()
{
    std::cout << "Home Work 21\n";
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
            check_lambda_issue();
            break;
        case 2:
            divide_by_number_in_vector();
            break;
        case 3:
            sort_strings_vec_by_length();
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
    std::cout << "1. check lambda issue\n";
    std::cout << "2. divide by number in vector\n";
    std::cout << "3. sort strings vec by length\n";
    std::cout << "4. Exit\n\n";

    std::cout << "Enter number:";
}

void check_lambda_issue()
{
    auto bad = makeLambda();
    std::cout << bad();
}

void divide_by_number_in_vector()
{
    const std::vector vec = { 10, 15, 20, 25, 30, 11, 17, 33 };
    std::cout << "vec: ";
    utl::print_arr(vec.data(), 8);
    std::cout << std::endl;
    constexpr int divisor = 5;

    const unsigned int count = countDivisibleBy(vec, divisor);
    std::cout << "elements divisible by " << divisor << " is " << count << std::endl;
}

bool compare_by_length(const std::string& str1, const std::string& str2)
{
    return str1.length() < str2.length();
}

class length_comparator
{
public:
    bool operator()(const std::string& str1, const std::string& str2) const
    {
        return str1.length() < str2.length();
    }
};

void sort_strings_vec_by_length()
{
	const std::vector<std::string> string_container = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

    std::vector<std::string> sorted_by_length_simple_func = string_container;
    std::sort(sorted_by_length_simple_func.begin(), sorted_by_length_simple_func.end(), compare_by_length);
    std::cout << "Sorted by length (simple function): \n";
    print_vector_strings(sorted_by_length_simple_func);
    std::cout << std::endl << std::endl;

    std::vector<std::string> sorted_by_length_func_obj = string_container;
    std::sort(sorted_by_length_func_obj.begin(), sorted_by_length_func_obj.end(), length_comparator());
    std::cout << "Sorted by length (functional object): \n";
    print_vector_strings(sorted_by_length_func_obj);
    std::cout << std::endl << std::endl;

    std::vector<std::string> sorted_by_length_lambda = string_container;
    std::sort(sorted_by_length_lambda.begin(), sorted_by_length_lambda.end(),
        [](const std::string& str1, const std::string& str2) {
            return str1.length() < str2.length();
        });
    std::cout << "Sorted by length (lambda function): \n";
    print_vector_strings(sorted_by_length_lambda);
}

void print_vector_strings(const std::vector<std::string>& vec)
{
    for (const auto& str : vec)
    {
        std::cout << str << " ";
    }
}
