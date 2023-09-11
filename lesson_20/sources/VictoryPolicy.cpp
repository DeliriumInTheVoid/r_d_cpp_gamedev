#include "ItemDeck.h"
#include "ModifierDeck.h"
#include "Munchkin.h"
#include "VictoryPolicy.h"

void VictoryPolicy_AddItems::apply(Munchkin* munchkin)
{
    for(int i = 0; i < m_itemsNum; ++i)
    {
        munchkin->addItem(m_itemsDeck.generateItem());
    }
}

std::string VictoryPolicy_AddItems::getFullInfo()
{
    return "you will be equipped with " + std::to_string(m_itemsNum) + " items";
}

void VictoryPolicy_AddModifiers::apply(Munchkin* munchkin)
{
    for (int i = 0; i < m_modifiersNum; ++i)
    {
        munchkin->addModifier(m_modifierDeck.generateModifier());
    }
}

std::string VictoryPolicy_AddModifiers::getFullInfo()
{
    return "to your hand will be add " + std::to_string(m_modifiersNum) + " modifiers";
}

void VictoryPolicy_AddLevel::apply(Munchkin* munchkin)
{
    munchkin->updateLevelBy(m_levelsNum);
}

std::string VictoryPolicy_AddLevel::getFullInfo()
{
    return "your level will be updated by " + std::to_string(m_levelsNum);
}
