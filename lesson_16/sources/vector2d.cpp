#include <cmath>

#include "../headers/vector2d.hpp"

vector2d operator+(const vector2d& left_vector, const vector2d& right_vector)
{
    return { left_vector.x_ + right_vector.x_, left_vector.y_ + right_vector.y_ };
}

vector2d operator-(const vector2d& left_vector, const vector2d& right_vector)
{
    return { left_vector.x_ - right_vector.x_, left_vector.y_ - right_vector.y_ };
}

std::ostream& operator<<(std::ostream& os, const vector2d& vec)
{
    os << "{" << vec.x_ << "; " << vec.y_ << "}";
    return os;
}

std::istream& operator>>(std::istream& is, vector2d& vec)
{
    is >> vec.x_ >> vec.y_;
    return is;
}

vector2d::vector2d(const float x, const float y) : x_{ x }, y_{ y }
{
}

vector2d& vector2d::operator=(const vector2d& other)
{
    x_ = other.x_;
    y_ = other.y_;
    return *this;
}

vector2d vector2d::operator+(const vector2d& other)
{
    return { x_ + other.x_, y_ + other.y_ };
}

vector2d vector2d::operator-(const vector2d& other)
{
    return { x_ - other.x_, y_ - other.y_ };
}

float vector2d::operator()() const
{
    return std::sqrt(x_ * x_ + y_ * y_);
}

float& vector2d::operator[](const int i)
{
    return i % 2 == 0 ? x_ : y_;
}
