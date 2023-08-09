#pragma once

constexpr int students_num = 5;
constexpr int student_marks = 4;
constexpr int max_average_mark = 5;

struct student
{
    const char* name{};
    float marks[student_marks] = {};
};

float average_mark(const student& student);

void print_student(const student& student);

void print_students(const student students[]);

void sort_students(student students[]);

student* best_student(student students[]);

int best_students_amount(student students[]);
