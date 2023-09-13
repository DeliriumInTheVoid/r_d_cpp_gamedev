#pragma once
#include <memory>
#include <vector>

class Item;

class ItemDeck
{
public:
    std::vector<std::shared_ptr<Item>> generateItems();
    std::shared_ptr<Item> generateItem();
    void addItem(std::shared_ptr<Item>&& item);

private:
    std::vector<std::shared_ptr<Item>> m_itemsDataBase;

private:
    unsigned m_usedItems{ 0 };
};
