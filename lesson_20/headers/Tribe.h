#pragma once

#include <string>

enum class Tribe : unsigned
{
    Human = 1,
    Undead,
    Zombie,
    God,
    Count
};

std::string getTribeString(Tribe tribe);

Tribe getTribe(std::string tribeStr);