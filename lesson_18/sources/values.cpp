#include <iostream>
#include <cstdlib>

#include "values.hpp"


diapason_value::diapason_value(const int min_value, const int max_value)
{
    if (max_value < min_value)
    {
        std::cerr << "diapason_value, c-tor(). error: wrong diapason: min="
            << min_value << ", max=" << max_value;
    }

    min_ = min_value;
    max_ = max_value;
}

int diapason_value::generate_value() const
{
    return min_ + std::rand() % (max_ - min_ + 1);
}

chanced_diapason_value::chanced_diapason_value(const int chance, diapason_value&& value) : chance_{ chance }, value_{ value }
{
    if (chance < 0 || chance > 100)
    {
        std::cerr << "chanced_value, c-tor(). error: chance must be [0..100]";
    }
}

int chanced_diapason_value::get_value(const int chance_increase) const
{
    return random_value_.generate_value() <= chance_ + chance_increase ? value_.generate_value() : 0;
}

chanced_value::chanced_value(const int chance, const float value) : chance_{ chance }, value_{ value }
{
}

float chanced_value::get_value() const
{
    return random_value_.generate_value() <= chance_ ? value_ : 0;
}
