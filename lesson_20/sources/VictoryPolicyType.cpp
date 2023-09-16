#include <string>
#include <algorithm>

#include "VictoryPolicyType.h"


VictoryPolicyType getVictoryPolicy(std::string action)
{
    std::ranges::transform(action, action.begin(), toupper);

    if (action == "ADD_ITEMS")
    {
        return VictoryPolicyType::AddItems;
    }

    if (action == "ADD_MODIFIERS")
    {
        return VictoryPolicyType::AddModifiers;
    }

    if (action == "UPDATE_LEVEL")
    {
        return VictoryPolicyType::UpdateLevel;
    }

    return VictoryPolicyType::None;
}
