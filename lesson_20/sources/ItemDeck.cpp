#include "ItemDeck.h"

#include "Item.h"

ItemDeck::ItemDeck()
{
    //TODO: Move item's database to file in format:
    // type;name; params for the type
    //For example:
    //UndeadWeapon;"Holy Grenade";4		//should call UndeadWeapon("Holy Grenade", 4);

    //TODO: Setup more items of diferent types
    m_itemsDataBase =
    { new Weapon{"The Sword of DOOM", 5},
    new UndeadWeapon{"Stinky knife", 2},
    new UndeadWeapon{"Holy grenade", 4} };
}

ItemDeck::~ItemDeck()
{
    //TODO: FREE MEMORY
}

std::vector<Item*> ItemDeck::generateItems() const
{
    //TODO: PICK AT RANDOM SEVERAL ITEMS FROM DATABASE AS A PLAYER HAND
    return m_itemsDataBase;
}
