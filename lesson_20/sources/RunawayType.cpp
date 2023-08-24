#include <string>
#include <algorithm>

#include "RunawayType.h"

RunawayType getRunawayType(std::string runaway)
{
    std::ranges::transform(runaway, runaway.begin(), toupper);

    if (runaway == "LEVEL_DOWNGRADE")
    {
        return RunawayType::LevelDowngrade;
    }

    if (runaway == "LEVEL_DOWNGRADE_IF")
    {
        return RunawayType::LevelDowngradeIf;
    }

    if (runaway == "REMOVE_ITEM")
    {
        return RunawayType::RemoveItems;
    }

    if (runaway == "REMOVE_MODIFIER")
    {
        return RunawayType::RemoveModifiers;
    }

    return RunawayType::None;
}