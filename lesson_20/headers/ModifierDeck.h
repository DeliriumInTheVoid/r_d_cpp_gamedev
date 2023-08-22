#pragma once
#include <vector>

//forward declaration to not include header into header which will increase compilation time
class Modifier;

class ModifierDeck
{
public:
    ModifierDeck();
    ~ModifierDeck();

    Modifier* generateModifier() const;

    std::vector<Modifier*> generateModifiers() const { return m_modifiersDatabase; }

private:
    std::vector<Modifier*> m_modifiersDatabase;
};
