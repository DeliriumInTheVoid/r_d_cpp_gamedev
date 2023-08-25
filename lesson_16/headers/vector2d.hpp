#pragma once

#include <iostream>

class vector2d
{
public:
    friend vector2d operator+(const vector2d& left_vector, const vector2d& right_vector);
    friend vector2d operator-(const vector2d& left_vector, const vector2d& right_vector);
    friend std::ostream& operator<<(std::ostream& os, const vector2d& vec);
    friend std::istream& operator>>(std::istream& is, vector2d& vec);

public:
    vector2d() = default;
    vector2d(const float x, const float y);

public:
    vector2d& operator=(const vector2d& other);
    vector2d operator+(const vector2d& other);
    vector2d operator-(const vector2d& other);
    float operator()() const;
    float& operator[](int i);

private:
    float x_{ 0 };
    float y_{ 0 };

};
