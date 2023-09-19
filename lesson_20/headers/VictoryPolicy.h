#pragma once

#include "MonsterPolicy.h"


class ModifierDeck;
class ItemDeck;
class Munchkin;

class VictoryPolicy_AddItems : public MonsterPolicy
{
public:
    VictoryPolicy_AddItems(ItemDeck& itemsDeck, const int itemsNum) : m_itemsNum{ itemsNum }, m_itemsDeck { itemsDeck }
    {}
    virtual ~VictoryPolicy_AddItems() override = default;

public:
    void apply(Munchkin* munchkin) override;
    virtual std::string getFullInfo() override;

private:
    int m_itemsNum;
    ItemDeck& m_itemsDeck;
};

class VictoryPolicy_AddModifiers : public MonsterPolicy
{
public:
    VictoryPolicy_AddModifiers(ModifierDeck& modifierDeck, const int modifiersNum)
        : m_modifiersNum{ modifiersNum }, m_modifierDeck{ modifierDeck }
    {}
    virtual ~VictoryPolicy_AddModifiers() override = default;

public:
    void apply(Munchkin* munchkin) override;
    virtual std::string getFullInfo() override;

private:
    int m_modifiersNum;
    ModifierDeck& m_modifierDeck;
};

class VictoryPolicy_AddLevel : public MonsterPolicy
{
public:
    VictoryPolicy_AddLevel(const int levelsNum) : m_levelsNum{ levelsNum }
    {}
    virtual ~VictoryPolicy_AddLevel() override = default;

public:
    void apply(Munchkin* munchkin) override;
    virtual std::string getFullInfo() override;

private:
    int m_levelsNum;
};