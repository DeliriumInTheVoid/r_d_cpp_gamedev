#include "Fight.h"

#include "Item.h"
#include "Modifier.h"
#include "Munchkin.h"
#include "Monster.h"
#include "Runaway.h"

void Fight::start()
{
    m_result = FightResult::InProgress;

    calculateMunchkinPower();
    calculateMonsterPower();
}

void Fight::applyModifier(int choice)
{
	const std::shared_ptr<Modifier> modifier = m_munchkin->popModifier(choice);
    if (modifier == nullptr)
    {
        //Give some assert/warning in debug to user that modifier is null so something is wrong
        return;
    }

    modifier->apply(m_munchkin, m_monster);

    m_MunchkinPower += modifier->getMunchkinModifier();
    m_monsterPower += modifier->getMonsterModifier();
}

void Fight::runawayFlow()
{
	const std::shared_ptr<MonsterPolicy> policy = m_monster->getRunawayPolicy();
    policy->apply(m_munchkin);

    m_result = FightResult::MonsterWon;
}

void Fight::victoryFlow()
{
    const std::shared_ptr<MonsterPolicy> policy = m_monster->getVictoryPolicy();
    policy->apply(m_munchkin);

    m_munchkin->updateLevelBy(1);

    m_result = FightResult::MunchkinWon;
}

void Fight::calculateMunchkinPower()
{
    m_MunchkinPower += m_munchkin->getLevel();
    for (auto& item : m_munchkin->getItems())
    {
        m_MunchkinPower += item->getPower(m_monster);
    }
}

void Fight::calculateMonsterPower()
{
    m_monsterPower = m_monster->getLevel();
}
