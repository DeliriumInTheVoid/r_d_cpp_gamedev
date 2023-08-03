#include <iostream>

#include "headers/student_utils.hpp"

int main()
{

    student students[students_num]
    {
        { "name1", { 3.0, 4.0, 3.0, 4.0 } },
        { "name2", { 3.0, 4.0, 4.0, 4.0 } },
        { "name3", { 4.0, 4.0, 5.0, 4.0 } },
        { "name4", { 5.0, 4.0, 5.0, 4.0 } },
        { "name5", { 5.0, 5.0, 5.0, 4.0 } }
    };

    std::cout << "students:\n";
    print_students(students);
    std::cout << std::endl << std::endl;

    const student* student = best_student(students);
    std::cout << "best student:\n";
    print_student(*student);
    std::cout << std::endl << std::endl;

    sort_students(students);
    std::cout << "sorted students:\n";
    print_students(students);
    std::cout << std::endl << std::endl;

    std::cout << "best students, average mark >= 75% => " << best_students_amount(students);
    std::cout << std::endl << std::endl;

    return 0;
}
