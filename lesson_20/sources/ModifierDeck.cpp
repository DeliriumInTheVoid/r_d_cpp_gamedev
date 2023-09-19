#include <memory>

#include "ModifierDeck.h"
#include "Modifier.h"

void ModifierDeck::addModifier(const std::shared_ptr<Modifier>& modifier)
{
    m_modifiersDatabase.push_back(modifier);
}

std::shared_ptr<Modifier> ModifierDeck::generateModifier()
{
    const size_t size = m_modifiersDatabase.size();
    if (m_usedModifiers >= size - 1)
    {
        m_usedModifiers = 0;
    }

    const unsigned index = std::rand() % (size - m_usedModifiers);
    auto monster = m_modifiersDatabase[index];

    ++m_usedModifiers;
    std::swap(m_modifiersDatabase[index], m_modifiersDatabase[size - m_usedModifiers]);

    return monster;
}

std::vector<std::shared_ptr<Modifier>> ModifierDeck::generateModifiers()
{
    unsigned constexpr minModifiers = 3;
    unsigned constexpr maxModifiers = 5;

    const unsigned modifierNum = minModifiers + std::rand() % (maxModifiers - minModifiers + 1);

    std::vector<std::shared_ptr<Modifier>> generatedModifiers{};
    generatedModifiers.reserve(modifierNum);

    for (size_t i = 0; i < modifierNum; ++i)
    {
        generatedModifiers.push_back(generateModifier());
    }

    return generatedModifiers;
}
