#include <string>
#include <algorithm>

#include "ModifierTarget.h"


ModifierTarget getTribeModifierTarget(std::string targetStr)
{
    std::ranges::transform(targetStr, targetStr.begin(), toupper);

    if (targetStr == "MUNCHKIN")
    {
        return ModifierTarget::Munchkin;
    }

    if (targetStr == "MONSTER")
    {
        return ModifierTarget::Monster;
    }

    return ModifierTarget::Unknown;
}

std::string getTargetString(ModifierTarget target)
{
    switch (target) {
    case ModifierTarget::Munchkin:
        return "Munchkin";
        break;
    case ModifierTarget::Monster:
        return "Monster";
        break;
    case ModifierTarget::Unknown:
    default:
        return "Unknown";
    }
}
