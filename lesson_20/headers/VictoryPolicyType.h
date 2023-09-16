#pragma once

enum class VictoryPolicyType
{
    None = 0,
    AddItems,
    AddModifiers,
    UpdateLevel
};

VictoryPolicyType getVictoryPolicy(std::string action);
