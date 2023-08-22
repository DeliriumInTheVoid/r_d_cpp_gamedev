#pragma once
#include <string>
#include "Tribe.h"
#include "Runaway.h"

class Runaway;

class Monster
{
public:
    Monster(const std::string& name, int level = 1, Tribe tribe = Tribe::Human, Runaway* policy = nullptr)
        : m_name(name), m_level(level), m_tribe(tribe), m_RunawayPolicy(policy) {}

    Tribe getTribe() const { return m_tribe; }
    unsigned int getLevel() const { return m_level; }
    const std::string& getName() const { return m_name; }

    Runaway* getRunawayPolicy() const { return m_RunawayPolicy; }

    std::string getFullInfo() const { return "Monster " + getName() + ", " + m_RunawayPolicy->getFullInfo(); }

private:
    unsigned int m_level = 1;
    Tribe m_tribe = Tribe::Human;
    std::string m_name;

    //#TODO: Add bonus victory policy for losing to munchkin similar to Runaway policies
    Runaway* m_RunawayPolicy = nullptr;
};
