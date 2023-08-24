#pragma once

#include <vector>
#include <memory>

//forward declaration to not include header into header which will increase compilation time
class Modifier;

class ModifierDeck
{
public:
    void addModifier(const std::shared_ptr<Modifier>& modifier);
    std::shared_ptr<Modifier> generateModifier();
    std::vector<std::shared_ptr<Modifier>> generateModifiers();

private:
    std::vector<std::shared_ptr<Modifier>> m_modifiersDatabase;
private:
    unsigned m_usedModifiers{ 0 };
};
