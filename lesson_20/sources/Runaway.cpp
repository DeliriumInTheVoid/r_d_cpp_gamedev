#include "Runaway.h"
#include "Munchkin.h"

std::string MonsterPolicy_Null::getFullInfo()
{
    return "";
}

void Runaway_LevelDowngrade::apply(Munchkin* munchkin)
{
    munchkin->updateLevelBy(-m_levelToDowngrade);
}

std::string Runaway_LevelDowngrade::getFullInfo()
{
    return "level is dropped to " + std::to_string(m_levelToDowngrade);
}

std::string Runaway_LevelDowngradeIf::getFullInfo()
{
	return Runaway_LevelDowngrade::getFullInfo() + 
        ", if your level more than " + std::to_string(m_minimalMunchkinLevelToApply);
}

void Runaway_LevelDowngradeIf::apply(Munchkin* munchkin)
{
    if (munchkin->getLevel() >= m_minimalMunchkinLevelToApply)
    {
        Runaway_LevelDowngrade::apply(munchkin);
    }
}

std::string Runaway_ModifierFromHandRemoval::getFullInfo()
{
    return "remove " + std::to_string(m_modifiersNum) + " modifiers from your hand";
}

void Runaway_ModifierFromHandRemoval::apply(Munchkin* munchkin)
{
    munchkin->removeModifierFromHand(m_modifiersNum);
}

std::string Runaway_ItemEquipedRemoval::getFullInfo()
{
    return "remove " + std::to_string(m_itemsNum) + " equipped items";
}

void Runaway_ItemEquipedRemoval::apply(Munchkin* munchkin)
{
    munchkin->removeItemEquipped(m_itemsNum);
}
