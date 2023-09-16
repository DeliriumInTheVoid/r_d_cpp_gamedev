#pragma once

#include <memory>
#include <string>

#include "Tribe.h"
#include "Runaway.h"


class MonsterPolicy;

class Monster
{
public:
    Monster(std::string name,
        const int level = 1,
        const Tribe tribe = Tribe::Human,
        std::shared_ptr<MonsterPolicy>&& runaway = nullptr,
        std::shared_ptr<MonsterPolicy>&& victory = nullptr)

        : m_level(level), m_tribe(tribe), m_name(std::move(name))
    {
        m_RunawayPolicy = runaway == nullptr ? std::make_shared<MonsterPolicy_Null>() : runaway;
        m_VictoryPolicy = victory == nullptr ? std::make_shared<MonsterPolicy_Null>() : victory;
    }

public:
    Tribe getTribe() const { return m_tribe; }
    unsigned int getLevel() const { return m_level; }
    const std::string& getName() const { return m_name; }

    std::shared_ptr<MonsterPolicy> getRunawayPolicy() const { return m_RunawayPolicy; }
    std::shared_ptr<MonsterPolicy> getVictoryPolicy() const { return m_VictoryPolicy; }

    std::string getFullInfo() const { return "Monster " + getName() + ", " + m_RunawayPolicy->getFullInfo(); }

private:
    unsigned int m_level = 1;
    Tribe m_tribe = Tribe::Human;
    std::string m_name;

    std::shared_ptr<MonsterPolicy> m_RunawayPolicy{ nullptr };
    std::shared_ptr<MonsterPolicy> m_VictoryPolicy{ nullptr };
};
