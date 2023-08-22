#pragma once
#include <string>
#include "Monster.h"
#include "Munchkin.h"

class Modifier
{
public:
    virtual void apply(Munchkin* munchkin, Monster* monster) = 0;
    virtual int getMunchkinModifier() const { return m_munchkinPowerChange; }
    virtual int getMonsterModifier() const { return m_monsterPowerChange; }

    virtual std::string getFullInfo() const { return ""; }

protected:
    int m_munchkinPowerChange = 0;
    int m_monsterPowerChange = 0;
};

class SimpleModifier : public Modifier
{
public:
    SimpleModifier(int powerBoost) : m_powerBoost(powerBoost) {}

    virtual void apply(Munchkin* munchkin, Monster* monster) override
    {
        m_munchkinPowerChange = m_powerBoost;
    }

    virtual std::string getFullInfo() const
    {
        return "Munchkin power +" + std::to_string(m_powerBoost);
    }

private:
    int m_powerBoost;
};

class DoubleMunchkinLevel : public Modifier
{
public:
    virtual void apply(Munchkin* munchkin, Monster* monster) override
    {
        m_munchkinPowerChange = munchkin->getLevel();
    }

    virtual std::string getFullInfo() const override
    {
        return "Double Munchkin level!";
    }
};

class HalvesMonsterLevel : public Modifier
{
public:
    //#TODO: cut monster's power in 2 if he is of type tribe

    HalvesMonsterLevel(Tribe tribe) {} //implement
    virtual void apply(Munchkin* munchkin, Monster* monster) override {} //implement

    virtual std::string getFullInfo() const override { return ""; } //implement
};

//#TODO: Add unique modifier munchkin/monster specific