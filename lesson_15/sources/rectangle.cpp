#include <iostream>

#include "../headers/rectangle.hpp"

rectangle::rectangle(const float length, const float height) : length_{ length }, height_{ height }
{
    if (length <= 0 || height <= 0)
    {
        std::cerr << "Wrong parameters for rectangle (" << length << "; " << height << "). Must be > 0!" << std::endl;
        length_ = 0;
        height_ = 0;
    }
    perimeter_ = length_ + length_ + height_ + height_;
    area_ = length_ * height_;
}

float rectangle::get_area() const
{
    return area_;
}

float rectangle::get_perimeter() const
{
    return perimeter_;
}
