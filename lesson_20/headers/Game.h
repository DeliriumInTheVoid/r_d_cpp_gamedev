#pragma once

#include <memory>

#include "Munchkin.h"
#include "ItemDeck.h"
#include "MonsterDeck.h"
#include "ModifierDeck.h"

class Game
{
public:
    Game() = default;
    void run();

private:
    void generateMunchkinInitialCards();
    std::weak_ptr<Monster> generateMonster();
    void loadData();

private:
    Munchkin m_munchkin;
    ItemDeck m_itemsDeck;
    ModifierDeck m_modifiersDeck;
    MonsterDeck m_monstersDeck;
};
