#pragma once

#include <string>

enum class AbilityAction : unsigned
{
    None = 0,
    Add,
    Multiply,
    Divide,
    Subtract
};

std::string getAbilityActionString(const AbilityAction action, const float value);

AbilityAction getAbilityAction(std::string action);
