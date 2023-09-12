#pragma once

#include <iostream>


enum class vector_relative_state
{
    identical,
    co_directed,
    opposite_directed,
    acute_angle,
    obtuse_angle,
    right_angle
};

class vector3d
{
public:
    friend std::ostream& operator<<(std::ostream& os, const vector3d& vec);

public:
    vector3d();
    vector3d(const float x, const float y, const float z);
    vector3d(const float x0, const float y0, const float z0, const float x1, const float y1, const float z1);

public:
    float dot_product(const vector3d& other) const;
    vector3d cross_product(const vector3d& other) const;
    vector3d negate() const;
    vector_relative_state get_relative_state(const vector3d& other) const;
    void scale(const float factor_x, const float factor_y, const float factor_z);
    float magnitude() const;

private:
    static bool equal(const float a, const float b, const float epsilon = std::numeric_limits<float>::epsilon());

private:
    float x_, y_, z_;
};
