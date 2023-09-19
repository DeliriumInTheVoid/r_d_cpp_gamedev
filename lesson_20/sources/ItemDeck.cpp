#include <memory>

#include "ItemDeck.h"

class Item;

std::vector<std::shared_ptr<Item>> ItemDeck::generateItems()
{
    unsigned constexpr minItems = 4;
    unsigned constexpr maxItems = 6;

    const unsigned itemNum = minItems + std::rand() % (maxItems - minItems + 1);

    std::vector<std::shared_ptr<Item>> generatedItems{};
    generatedItems.reserve(itemNum);

    for (size_t i = 0; i < itemNum; ++i)
    {
        generatedItems.push_back(generateItem());
    }

    return generatedItems;
}

std::shared_ptr<Item> ItemDeck::generateItem()
{
    const size_t size = m_itemsDataBase.size();
    if (m_usedItems >= size - 1)
    {
        m_usedItems = 0;
    }

    const unsigned index = std::rand() % (size - m_usedItems);
    auto item = m_itemsDataBase[index];

    ++m_usedItems;
    std::swap(m_itemsDataBase[index], m_itemsDataBase[size - m_usedItems]);

    return item;
}

void ItemDeck::addItem(std::shared_ptr<Item>&& item)
{
    m_itemsDataBase.push_back(item);
}
