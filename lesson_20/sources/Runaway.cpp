#include "Runaway.h"
#include "Munchkin.h"

void Runaway_LevelDowngrade::apply(Munchkin* munchkin)
{
    munchkin->updateLevelBy(-m_levelToDowngrade);
}

void Runaway_LevelDowngradeIf::apply(Munchkin* munchkin)
{
    if (munchkin->getLevel() >= m_minimalMunchkinLevelToApply)
    {
        Runaway_LevelDowngrade::apply(munchkin);
    }
}
