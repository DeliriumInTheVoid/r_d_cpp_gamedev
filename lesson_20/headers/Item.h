#pragma once

#include <memory>
#include <string>

#include "Monster.h"
#include "AbilityAction.h"


class WeaponSkill
{
public:
    virtual ~WeaponSkill() = default;

public:
    virtual int getPower(const int weaponPower, const std::weak_ptr<Monster>& monster) const = 0;
    virtual std::string getFullInfo() const = 0;
};

class WeaponSkillNull final : public WeaponSkill
{
public:
    virtual ~WeaponSkillNull() override = default;

public:
    virtual int getPower(const int weaponPower, const std::weak_ptr<Monster>& monster) const override
    {
        return weaponPower;
    }

    virtual std::string getFullInfo() const override
    {
        return "None";
    }
};

class WeaponKillMonster final : public WeaponSkill
{
public:
    WeaponKillMonster(std::string monsterName) : m_monsterName{std::move(monsterName)}
	{}
    virtual ~WeaponKillMonster() override = default;

public:
    virtual int getPower(const int weaponPower, const std::weak_ptr<Monster>& monster) const override
    {
        const auto monster_sh = monster.lock();
        return monster_sh->getName() == m_monsterName ? monster_sh->getLevel() : weaponPower;
    }

    virtual std::string getFullInfo() const override
    {
        return "Kill monster " + m_monsterName;
    }

private:
    std::string m_monsterName;
};

class WeaponSkillPowerBooster : public WeaponSkill
{
public:
    WeaponSkillPowerBooster(const AbilityAction action, const float value) :
        m_action{ action }, m_value{ value }
    {}

    virtual ~WeaponSkillPowerBooster() override = default;

public:
    using enum AbilityAction;

    virtual int getPower(const int weaponPower, const std::weak_ptr<Monster>& monster) const override
    {
        switch (m_action)
        {
        case Add:
            return static_cast<int>(static_cast<float>(weaponPower) + m_value);
            break;
        case Multiply:
            return static_cast<int>(static_cast<float>(weaponPower) * m_value);
            break;
        case None:
        default:
            return weaponPower;
        }
    }

    virtual std::string getFullInfo() const override
    {
        return getAbilityActionString(m_action, m_value);
    }

private:
    AbilityAction m_action;
    float m_value;
};

class WeaponSkillTribePowerBooster : public WeaponSkillPowerBooster
{
public:
    WeaponSkillTribePowerBooster(const Tribe tribe, const AbilityAction action, const float value) :
        WeaponSkillPowerBooster(action, value), m_tribe{ tribe }
    {}

    virtual ~WeaponSkillTribePowerBooster() override = default;

public:
    using enum AbilityAction;

    virtual int getPower(const int weaponPower, const std::weak_ptr<Monster>& monster) const override
    {
        if (monster.lock()->getTribe() == m_tribe)
        {
            return WeaponSkillPowerBooster::getPower(weaponPower, monster);
        }

        return weaponPower;
    }

    virtual std::string getFullInfo() const override
    {
        return WeaponSkillPowerBooster::getFullInfo() + " vs " + getTribeString(m_tribe) + "!";
    }

private:
    Tribe m_tribe{ Tribe::Count };
};

class Item
{
public:
    virtual ~Item() = default;
public:
    virtual int getBasePower() const = 0;
    virtual std::string getFullInfo() const = 0;

    virtual int getPower(const std::weak_ptr<Monster>& monster) const { return getBasePower(); }

    void setName(const std::string& name) { m_name = name; }
    const std::string getName() const { return m_name; }
    

private:
    std::string m_name;
    //other fields like description, image, category

private:
};

class Weapon : public Item
{
public:
    Weapon(const std::string& name, int power, std::unique_ptr<WeaponSkill> skill = nullptr)
    {
        setName(name);
        m_power = power;

        m_skill = skill == nullptr ? std::make_unique<WeaponSkillNull>() : std::move(skill);
    }

    virtual ~Weapon() override = default;

public:
    virtual int getPower(const std::weak_ptr<Monster>& monster) const override
    {
        return m_skill->getPower(getBasePower(), monster);
    }

    int getBasePower() const override
    {
        return m_power;
    }

    virtual std::string getFullInfo() const override
    {
        return "\"" + getName() + "\"" + ", power:" + std::to_string(getBasePower()) + ", skills: " + m_skill->getFullInfo() + "\n";
    }

private:
    int m_power = 0;
    std::unique_ptr<WeaponSkill> m_skill;
};
