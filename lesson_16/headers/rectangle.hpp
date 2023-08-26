#pragma once


class rectangle
{
public:
    rectangle();
    rectangle(const float length, const float height);

    ~rectangle();

public:
    float get_area() const;
    float get_perimeter() const;

public:
    static unsigned get_instances_count();

private:
    float length_{ 0 };
    float height_{ 0 };

    float perimeter_{ 0 };
    float area_{ 0 };

private:
    static unsigned instances_count_;

};
