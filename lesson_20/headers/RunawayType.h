#pragma once

#include <string>

enum class RunawayType : unsigned
{
    None = 0,
    LevelDowngrade,
    LevelDowngradeIf,
    RemoveItems,
    RemoveModifiers
};

RunawayType getRunawayType(std::string runaway);
