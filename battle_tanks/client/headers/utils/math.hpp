#pragma once

#include <numbers>
#include <cmath>

namespace bt
{
    constexpr float deg_to_rad_multiplier = static_cast<float>(std::numbers::pi / 180.0);
    constexpr float rad_to_deg_multiplier = static_cast<float>(180.0 / std::numbers::pi);

    inline float deg_to_rad(const float rotation)
    {
        return deg_to_rad_multiplier * rotation;
    }

    inline float rad_to_deg(const float rotation)
    {
        return rad_to_deg_multiplier * rotation;
    }

    inline float rad_to_deg_360(const float rotation)
    {
        return std::fmod(rad_to_deg(rotation), 360);
    }
}