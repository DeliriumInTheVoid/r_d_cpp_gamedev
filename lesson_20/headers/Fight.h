#pragma once

class Munchkin;
class Monster;

enum class FightResult
{
    InProgress,
    MunchkinWon,
    MonsterWon
};

class Fight
{
public:
    void setMunchkin(Munchkin* munchkin) { m_munchkin = munchkin; };
    void setMonster(Monster* monster) { m_monster = monster; }

    void start();
    bool getFinish() const { return m_result != FightResult::InProgress; }

    void applyModifier(int choice);

    int getMunchkinMonsterPowerDifference() const { return m_MunchkinPower - m_monsterPower; }
    
    int getMunchkinPower() const { return m_MunchkinPower; }
    int getMonsterPower() const { return m_monsterPower; }

    void runawayFlow();
    void victoryFlow();

private:
    void calculateMunchkinPower();
    void calculateMonsterPower();

private:
    int m_MunchkinPower = 0;
    int m_monsterPower = 0;

    Munchkin* m_munchkin = nullptr;
    Monster* m_monster = nullptr;

    FightResult m_result = FightResult::InProgress;
};
