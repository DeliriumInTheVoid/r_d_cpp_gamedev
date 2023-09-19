#include "Tribe.h"

#include <algorithm>

std::string getTribeString(Tribe tribe)
{
    switch (tribe)
    {
    case Tribe::Human: return "HUMAN";
    case Tribe::Undead: return "UNDEAD";
    case Tribe::Zombie: return "ZOMBIE";
    case Tribe::God: return "GOD";
    case Tribe::Count:
    default:
        return "";
    }
}

Tribe getTribe(std::string tribeStr)
{
    std::ranges::transform(tribeStr, tribeStr.begin(), toupper);

    if (tribeStr == "HUMAN")
    {
        return Tribe::Human;
    }

    if (tribeStr == "UNDEAD")
    {
        return Tribe::Undead;
    }

    if (tribeStr == "ZOMBIE")
    {
        return Tribe::Zombie;
    }

    if (tribeStr == "GOD")
    {
        return Tribe::God;
    }

    return Tribe::Count;
}
