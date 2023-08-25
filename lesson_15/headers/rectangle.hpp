#pragma once


class rectangle
{
public:
    rectangle() = default;
    rectangle(const float length, const float height);

public:
    float get_area() const;
    float get_perimeter() const;

private:
    float length_{ 0 };
    float height_{ 0 };

    float perimeter_{ 0 };
    float area_{ 0 };
};
