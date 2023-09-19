#pragma once

#include <string>


enum class ModifierTarget : unsigned
{
    Unknown = 0,
    Munchkin,
    Monster
};

ModifierTarget getTribeModifierTarget(std::string targetStr);

std::string getTargetString(ModifierTarget target);
