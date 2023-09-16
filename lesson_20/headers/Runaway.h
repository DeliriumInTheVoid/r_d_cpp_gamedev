#pragma once

#include <string>

#include "MonsterPolicy.h"


//forwad declaration
class Munchkin;

class MonsterPolicy_Null final : public MonsterPolicy
{
public:
    virtual ~MonsterPolicy_Null() override = default;

public:
    virtual void apply(Munchkin* munchkin) override
    {}

    virtual std::string getFullInfo() override;
};

class Runaway_LevelDowngrade : public MonsterPolicy
{
public:
    Runaway_LevelDowngrade(const int level) : m_levelToDowngrade{ level }
	{}
    virtual ~Runaway_LevelDowngrade() override = default;

public:
    void apply(Munchkin* munchkin) override;
    virtual std::string getFullInfo() override;

protected:
    int m_levelToDowngrade;
};

class Runaway_LevelDowngradeIf : public Runaway_LevelDowngrade
{
public:
    Runaway_LevelDowngradeIf(const int level, const int minimalMunchkinLevel) 
        : Runaway_LevelDowngrade(level), m_minimalMunchkinLevelToApply{ minimalMunchkinLevel }
    {}
    virtual ~Runaway_LevelDowngradeIf() override = default;
public:
    virtual std::string getFullInfo() override;
    void apply(Munchkin* munchkin) override;

private:
    int m_minimalMunchkinLevelToApply;
};

//Remove a card from hand(modifier) at random
class Runaway_ModifierFromHandRemoval : public MonsterPolicy
{
public:
    Runaway_ModifierFromHandRemoval(const unsigned removeModifiersNum)
        : m_modifiersNum{ removeModifiersNum }
    {}
    virtual ~Runaway_ModifierFromHandRemoval() override = default;

public:
    virtual std::string getFullInfo() override;
    void apply(Munchkin* munchkin) override;

private:
    unsigned m_modifiersNum;
};

//Remove equiped item from Outfit with biggest base power
class Runaway_ItemEquipedRemoval : public MonsterPolicy
{
public:
    Runaway_ItemEquipedRemoval(const unsigned removeModifiersNum)
        : m_itemsNum{ removeModifiersNum }
    {}
    virtual ~Runaway_ItemEquipedRemoval() override = default;

public:
    virtual std::string getFullInfo() override;
    void apply(Munchkin* munchkin) override;

private:
    unsigned m_itemsNum;
};
