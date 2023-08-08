#include "../headers/student_utils.hpp"

#include <algorithm>
#include <iostream>

float average_mark(const student& student)
{
    float sum{ 0 };

    for (int i = 0; i < student_marks; ++i)
    {
        sum += student.marks[i];
    }

    return sum / student_marks;
}

void print_student(const student& student)
{
    std::cout << "name: " << student.name << "; marks: [";
    for (int i = 0; i < student_marks; ++i)
    {
        std::cout << student.marks[i] << ", ";
    }

    std::cout << "]; average: " << average_mark(student) << ";";
}

void print_students(const student students[])
{
    for (int i = 0; i < students_num; ++i)
    {
        print_student(students[i]);
        std::cout << std::endl;
    }
}

void sort_students(student students[])
{
    std::sort(&students[0], &students[students_num], [](const student& item1, const student& item2)
        {
            return average_mark(item1) > average_mark(item2);
        });
}

student* best_student(student students[])
{
    int index{ -1 };
    float max_mark{ -1 };
    for(int i = 0; i < students_num; ++i)
    {
        const float student_average{ average_mark(students[i]) };

        if (student_average > max_mark)
        {
            max_mark = student_average;
            index = i;
        }
    }

    return &students[index];
}

int best_students_amount(student students[])
{
    int best_students_amount{ 0 };
    for(int i = 0; i < students_num; ++i)
    {
        const float student_average{ average_mark(students[i]) };
        if (student_average / max_average_mark >= 0.75)
        {
            ++best_students_amount;
        }
    }

    return best_students_amount;
}
