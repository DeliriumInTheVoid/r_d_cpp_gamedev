#include <cmath>

#include "vector3d.hpp"


vector3d::vector3d() : vector3d(0, 0, 0)
{
}

vector3d::vector3d(const float x, const float y, const float z) : vector3d(0, 0, 0, x, y, z)
{
}

vector3d::vector3d(const float x0, const float y0, const float z0, const float x1, const float y1, const float z1)

{
    x_ = x1 - x0;
    y_ = y1 - y0;
    z_ = z1 - z0;
}

float vector3d::dot_product(const vector3d& other) const
{
    return x_ * other.x_ + y_ * other.y_ + z_ * other.z_;
}

vector3d vector3d::cross_product(const vector3d& other) const
{
    const float x = y_ * other.z_ - z_ * other.y_;
    const float y = z_ * other.x_ - x_ * other.z_;
    const float z = x_ * other.y_ - y_ * other.x_;

    return { x, y, z };
}

vector3d vector3d::negate() const
{
    return { -x_, -y_, -z_ };
}

vector_relative_state vector3d::get_relative_state(const vector3d& other) const
{
    if (equal(x_, other.x_) && equal(y_, other.y_) && equal(z_, other.z_))
    {
        return vector_relative_state::identical;
    }

    const float dot = dot_product(other);

    if (equal(dot, 1.0))
    {
        return vector_relative_state::co_directed;
    }

    if (equal(dot, -1.0))
    {
        return vector_relative_state::opposite_directed;
    }

    if (equal(dot, 0))
    {
        return vector_relative_state::obtuse_angle;
    }

    if (dot < 0)
    {
        return vector_relative_state::right_angle;
    }

    return vector_relative_state::acute_angle;
}

void vector3d::scale(const float factor_x, const float factor_y, const float factor_z)
{
    x_ *= factor_x;
    y_ *= factor_y;
    z_ *= factor_z;
}

float vector3d::magnitude() const
{
    return sqrt(x_ * x_ + y_ * y_ + z_ * z_);
}

bool vector3d::equal(const float a, const float b, const float epsilon)
{
    return std::abs(a - b) <= epsilon;
}

std::ostream& operator<<(std::ostream& os, const vector3d& vec)
{
    os << "(" << vec.x_ << ", " << vec.y_ << ", " << vec.z_ << ")";
    return os;
}
