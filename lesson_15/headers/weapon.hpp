#pragma once

#include <iostream>
#include <string_view>

class weapon
{
public:
    friend std::ostream& operator << (std::ostream& os, weapon& instance);

public:
    weapon() = default;
    weapon(std::string_view name, const float damage, const float range);

public:
    std::string_view get_name() const;
    float get_damage() const;
    float get_range() const;

private:
    std::string_view name_;
    float damage_{ 0 };
    float range_{ 0 };
};
