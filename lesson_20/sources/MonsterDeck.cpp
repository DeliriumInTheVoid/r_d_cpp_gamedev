#include "MonsterDeck.h"

class Monster;

std::shared_ptr<Monster> MonsterDeck::generateMonster()
{
    const size_t size = m_monstersDatabase.size();
    if (m_usedMonsters >= size - 1)
    {
        m_usedMonsters = 0;
    }

    const unsigned index = std::rand() % (size - m_usedMonsters);
    auto monster = m_monstersDatabase[index];

    ++m_usedMonsters;
    std::swap(m_monstersDatabase[index], m_monstersDatabase[size - m_usedMonsters]);

    return monster;
}

void MonsterDeck::addMonster(std::shared_ptr<Monster>&& monster)
{
    m_monstersDatabase.push_back(monster);
}
