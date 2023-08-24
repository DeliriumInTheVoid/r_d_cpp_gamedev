#include <memory>

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

void Munchkin::addItem(const std::shared_ptr<Item>& item)
{
    m_items.push_back(item);
}

void Munchkin::setItems(const std::vector<std::shared_ptr<Item>>& items)
{
    m_items = items;
}

const std::vector<std::shared_ptr<Item>>& Munchkin::getItems() const
{
    return m_items;
}

void Munchkin::removeModifierFromHand(unsigned modifiersNum)
{
    if (m_modifiers.empty())
    {
        return;
    }

    if (m_modifiers.size() > modifiersNum)
    {
	    while (modifiersNum--)
	    {
		    const unsigned index = std::rand() % m_modifiers.size();
            m_modifiers.erase(m_modifiers.begin() + index);
	    }
    }
    else
    {
        m_modifiers.clear();
    }
}

void Munchkin::removeItemEquipped(unsigned itemsNum)
{
    if (m_items.empty())
    {
        return;
    }

    if (m_items.size() > itemsNum)
    {
        while (itemsNum--)
        {
            const unsigned index = std::rand() % m_items.size();
            m_items.erase(m_items.begin() + index);
        }
    }
    else
    {
        m_items.clear();
    }
}

std::shared_ptr<Modifier> Munchkin::popModifier(const unsigned idx)
{
    if (idx >= m_modifiers.size() || idx < 0)
    {
        return nullptr;
    }

    std::shared_ptr<Modifier> modifier = m_modifiers[idx];
    m_modifiers.erase(m_modifiers.begin() + idx);
    return modifier;
}

void Munchkin::addModifier(const std::shared_ptr<Modifier>& modifier)
{
    m_modifiers.push_back(modifier);
}
