#pragma once

#include <memory>
#include <vector>


class Monster;

class MonsterDeck
{
public:
    std::shared_ptr<Monster> generateMonster();
    void addMonster(std::shared_ptr<Monster>&& monster);

private:
    std::vector<std::shared_ptr<Monster>> m_monstersDatabase;

private:
    unsigned m_usedMonsters{ 0 };
};
