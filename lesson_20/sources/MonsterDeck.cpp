#include "MonsterDeck.h"

#include "Monster.h"
#include "Runaway.h"

MonsterDeck::MonsterDeck()
{
    m_monstersDatabase =
    {
        //#TODO: Think of new monsters, feel free to use official Munchkin games as a reference
        new Monster{"Crazy Joe", 4},
        new Monster{"Vampire", 3, Tribe::Undead, new Runaway_LevelDowngrade{1}},
        new Monster{"Shiva destructor", 20, Tribe::God, new Runaway_LevelDowngradeIf{2, 5}}
    };
}

MonsterDeck::~MonsterDeck()
{
    //TODO: Clear memory
}

Monster* MonsterDeck::generateMonster() const
{
    //#TODO: this call should return monster every time
    //either for as long as the same game is being played
    //or unless ALL cards were generated from database to the game - in this case 
    //make ALL cards available again

    const int choice = std::rand() % m_monstersDatabase.size();
    return m_monstersDatabase[choice];
}
