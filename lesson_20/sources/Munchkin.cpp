#include "Munchkin.h"
#include "Item.h"
#include "Modifier.h"

void Munchkin::updateLevelBy(int levels)
{
    if (m_level + levels <= 0)
    {
        m_level = 1;
    }
    else
    {
	    m_level += levels; 
    }

}

void Munchkin::addItem(Item* item)
{
    m_items.push_back(item);
}

void Munchkin::addItems(const std::vector<Item*>& items)
{
    m_items = items;
}

const std::vector<Item*>& Munchkin::getItems() const
{
    return m_items;
}

Modifier* Munchkin::popModifier(int idx)
{
    if (idx >= m_modifiers.size() || idx < 0)
    {
        return nullptr;
    }

    Modifier* modifier = m_modifiers[idx];
    m_modifiers.erase(m_modifiers.begin() + idx);
    return modifier;

}
