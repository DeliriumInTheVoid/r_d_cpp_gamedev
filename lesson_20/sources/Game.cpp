#include "Game.h"

#include <iostream>
#include <memory>
#include <chrono> //for sleep delays
#include <thread> //for sleep delays
#include <time.h> //for rand generator init

#include "Fight.h"
#include "Item.h"
#include "ItemDeck.h"
#include "Modifier.h"
#include "Monster.h"
#include "GameDataLoader.h"

namespace UI
{
    void printDelayWithText(const int count, const std::string& text)
    {
        std::cout << text;

        for (int i = 0; i < count; i++)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(700));
            std::cout << ".";
        }
        std::cout << std::endl;
    }

void printTossing()
{
    UI::printDelayWithText(5, "Tossing the deck");
}

void printMonsterSelection()
{
    UI::printDelayWithText(5, "Selecting monster card");
}

void printMunchkinWon(const std::shared_ptr<Monster>& monster)
{
    std::cout << "CONGRATS: Munchkin WON!\n";
    std::cout << monster->getRunawayPolicy()->getFullInfo() << std::endl;
}

void pressAnyKeyToContinue()
{
    //Can use platform specific system functions to 
    //receive input directly from the keyboard without 
    //its output to console and approval with Enter
    std::cout << "\nPress any key to start a round..." << std::flush;
    std::cin.clear();
    std::cin.get();
}

void printMunchkinLost(const std::shared_ptr<Monster>& monster)
{
    std::cout << "-------YOU'VE LOST to \"" << monster->getName() << "\"Monster!---------\n";
    std::cout << monster->getRunawayPolicy()->getFullInfo() << std::endl;
}

void printIntro(Munchkin* munchkin)
{
    std::cout << "WELCOME TO TEXT-BASED MUNCHKIN GAME!!!\n";
    std::cout << "Enter your Munchkin name: ";
    std::string name;
    std::cin >> name;
    munchkin->setName(name);

    UI::printTossing();
}

void printItems(const Munchkin* const munchkin)
{
    const std::vector<std::shared_ptr<Item>>& items = munchkin->getItems();
    std::cout << "--------OUTFIT-----------\n";
    for (auto& item : items)
    {
        std::cout << item->getFullInfo();
    }
}

void printModifiers(const Munchkin* munchkin)
{
    const auto& modifiers = munchkin->getModifiers();
    std::cout << "\n---------HAND CARDS----------\n";
    int idx = 1;
    for (auto& modifier : modifiers)
    {
        std::cout << idx++ << ". " << modifier->getFullInfo() << std::endl;
    }
}

void printPlayerDeck(const Munchkin* munchkin)
{
    std::cout << std::endl << "\nMunchkin " << "\"" << munchkin->getName() << "\", level " << munchkin->getLevel() << std::endl;
    printItems(munchkin);
    printModifiers(munchkin);
}

void printMunchkinPower(const Munchkin* munchkin, const Fight* fight)
{
    std::cout << "-----\"" << munchkin->getName() << "\" power: " << fight->getMunchkinPower() << "-------\n";
}

void printMonsterPower(const std::shared_ptr<Monster>& monster, const Fight* fight)
{
    std::cout << "-----\"" << monster->getName() << "\" power: " << fight->getMonsterPower() << "-------\n";
}

enum class UserInput
{
    None,
    Runaway,
    ApplyModifier
};

UserInput readUserBehavior()
{
    std::cout << "Use card from Hand or Runaway?(M/R)";
    char choice;
    std::cin >> choice;
    choice = std::toupper(choice);

    switch (choice)
    {
    case 'R':
        return UserInput::Runaway;
    case 'M':
        return UserInput::ApplyModifier;
    default:
        return UserInput::None;
    }
}

int selectModifierFromHand(unsigned short handSize)
{
    std::cout << "Select card from HAND: ";
    int choice = -1;
    std::cin >> choice;

    if (choice >= handSize || choice <= 0)
    {
        choice = -1;
    }

    return choice;
}

void printCurrentFightResult(const int powerDifference)
{
    std::cout << "You are missing " << std::abs(powerDifference) << " power!\n\n";
}

} //namespace UI


void printMonsterInfo(const std::shared_ptr<Monster>& monster)
{
    std::cout << "\n--------Monster \"" << monster->getName() << "\"" << ", of " <<
        getTribeString(monster->getTribe()) << ", level " << monster->getLevel() << " --------" << std::endl;
    std::cout << "Runaway: " << monster->getRunawayPolicy()->getFullInfo() << std::endl;
    std::cout << "Victory: " << monster->getVictoryPolicy()->getFullInfo() << std::endl << std::endl;
}

void Game::run()
{
    constexpr uint WinningLevel = 10;

    loadData();

    std::srand(static_cast<int>(std::time(0)));
    UI::printIntro(&m_munchkin);
    
    generateMunchkinInitialCards();

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (m_munchkin.getLevel() < WinningLevel)
    {
        UI::printPlayerDeck(&m_munchkin);
        
        UI::pressAnyKeyToContinue();
        UI::printMonsterSelection();

        std::shared_ptr<Monster> monster = generateMonster();
        printMonsterInfo(monster);

        Fight fight;
        fight.setMonster(monster);
        fight.setMunchkin(&m_munchkin);

        fight.start();

        //State pattern may be a good candidate here
        //Every case may be its own state with transition rules, e.g.
        //Start->InProgress->Win/Runaway/ApplyModifiers, Runaway->Lost, ApplyModifiers->InProgress
        //https://refactoring.guru/uk/design-patterns/state
        
        //Fight loop
        while (!fight.getFinish())
        {
            const int munchkinMonsterDifference = fight.getMunchkinMonsterPowerDifference();
            UI::printMunchkinPower(&m_munchkin, &fight);
            UI::printMonsterPower(monster, &fight);

            if (munchkinMonsterDifference > 0)
            {
                fight.victoryFlow();
                UI::printMunchkinWon(monster);
                UI::pressAnyKeyToContinue();
            }
            else
            {
                UI::printCurrentFightResult(munchkinMonsterDifference);
                UI::UserInput input = UI::readUserBehavior();
                if (input == UI::UserInput::Runaway)
                {
                    fight.runawayFlow();

                    UI::printMunchkinLost(monster);
                    UI::pressAnyKeyToContinue();
                }
                else if (input == UI::UserInput::ApplyModifier)
                {
                    UI::printModifiers(&m_munchkin);
                    const int choice = UI::selectModifierFromHand(static_cast<short>(m_munchkin.getModifiers().size()));
                    
                    if (choice != -1)
                        fight.applyModifier(choice - 1);
                }
            }
        }

    }
}

void Game::generateMunchkinInitialCards()
{
    m_munchkin.setItems(m_itemsDeck.generateItems());
    m_munchkin.addModifiers(m_modifiersDeck.generateModifiers());
}

std::shared_ptr<Monster> Game::generateMonster()
{
    return m_monstersDeck.generateMonster();
}

void Game::loadData()
{
    GameDataLoader loader;

    loader.loadMonsterDeck(m_monstersDeck, m_itemsDeck, m_modifiersDeck);
    loader.loadItemsDeck(m_itemsDeck);
    loader.loadModifiersDeck(m_modifiersDeck);
}
