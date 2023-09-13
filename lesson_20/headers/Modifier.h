#pragma once

#include <string>

#include "Monster.h"
#include "Munchkin.h"
#include "AbilityAction.h"
#include "ModifierTarget.h"


class Modifier
{
public:
    Modifier(std::string&& name, ModifierTarget target, const float value, const AbilityAction action)
        : m_name{ name }, m_target{ target }, m_value{ value }, m_action{ action }
    {}
    virtual ~Modifier() = default;
public:
    std::string getName() const { return m_name; }
    void apply(const Munchkin* munchkin, const std::weak_ptr<Monster>& monster)
    {
        if (m_target == ModifierTarget::Unknown)
        {
            return;
        }

        return calculateModifierChange(
            munchkin,
            monster,
            m_target == ModifierTarget::Monster ? m_monsterPowerChange : m_munchkinPowerChange
        );
    }

    virtual int getMunchkinModifier() const { return m_munchkinPowerChange; }
    virtual int getMonsterModifier() const { return m_monsterPowerChange; }

    virtual std::string getFullInfo() const { return ""; }

protected:
    virtual void calculateModifierChange(const Munchkin* const munchkin, const std::weak_ptr<Monster>& monster, int& modifierChange) = 0;

protected:
    std::string m_name;
    ModifierTarget m_target;
    float m_value;
    AbilityAction m_action;

    int m_munchkinPowerChange = 0;
    int m_monsterPowerChange = 0;
};

class Modifier_Power : public Modifier
{
public:
    Modifier_Power(std::string&& name, ModifierTarget target,  const float value, const AbilityAction action)
        : Modifier(std::move(name), target, value, action )
    {
    }

    virtual ~Modifier_Power() override = default;

public:
    virtual void calculateModifierChange(const Munchkin* const munchkin, const std::weak_ptr<Monster>& monster, int& modifierChange) override
    {
        switch (m_action)
        {
        case AbilityAction::Add:
            modifierChange = m_value;
            break;
        case AbilityAction::Subtract:
            modifierChange = -m_value;
            break;
        case AbilityAction::None:
        default: ;
        }
    }

    virtual std::string getFullInfo() const
    {
        return "\"" + getName() +"\", " + getTargetString(m_target) + " power " + getAbilityActionString(m_action, m_value);
    }
};


class Modifier_Level : public Modifier
{
public:
    Modifier_Level(std::string&& name, ModifierTarget target, const float value, const AbilityAction action)
        : Modifier(std::move(name), target, value, action)
    {}

    virtual ~Modifier_Level() override = default;
public:
    virtual void calculateModifierChange(const Munchkin* const munchkin, const std::weak_ptr<Monster>& monster, int& modifierChange) override
    {
        const unsigned targetLevel = m_target == ModifierTarget::Munchkin ? munchkin->getLevel() : monster.lock()->getLevel();

        switch (m_action)
        {
        case AbilityAction::Add:
            modifierChange = static_cast<int>(m_value);
            break;
        case AbilityAction::Subtract:
            modifierChange = static_cast<int>(- m_value);
            break;
        case AbilityAction::Multiply:
            modifierChange = static_cast<int>(static_cast<float>(targetLevel) * (m_value - 1));
            break;
        case AbilityAction::Divide:
            modifierChange = -static_cast<int>(targetLevel - static_cast<int>(static_cast<float>(targetLevel) / m_value));
            break;
        case AbilityAction::None:
        default:;
        }
    }

    virtual std::string getFullInfo() const override
    {
        return "\"" + getName() + "\", " + getTargetString(m_target) + " level " + getAbilityActionString(m_action, m_value);
    }

};

class Modifier_LevelByTribe : public Modifier_Level
{
public:
    Modifier_LevelByTribe(std::string&& name, ModifierTarget target, const float value, const AbilityAction action, const Tribe tribe)
        : Modifier_Level(std::move(name), target, value, action), m_tribe{ tribe }
    {}

    virtual ~Modifier_LevelByTribe() override = default;
public:
    virtual void calculateModifierChange(const Munchkin* const munchkin, const std::weak_ptr<Monster>& monster, int& modifierChange) override
    {
        if (monster.lock()->getTribe() != m_tribe)
        {
            return;
        }

        Modifier_Level::calculateModifierChange(munchkin, monster, modifierChange);
    }

    virtual std::string getFullInfo() const override
    {
        return Modifier_Level::getFullInfo() + " vs " + getTribeString(m_tribe);
    }

private:
    Tribe m_tribe;
};
