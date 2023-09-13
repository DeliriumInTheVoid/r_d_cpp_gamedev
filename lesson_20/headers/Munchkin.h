#pragma once

#include <memory>
#include <vector>
#include <string>

class Item;
class Modifier;

using uint = unsigned int;

class Munchkin
{
public:
    Munchkin() = default;
    Munchkin(const std::string& name) : m_name(name) {}

    void setName(const std::string& name) { m_name = name; }
    const std::string& getName() const { return m_name; }

    int getLevel() const { return m_level; }
    void updateLevelBy(int levels);

    void addItem(const std::shared_ptr<Item>& item);
    void setItems(const std::vector<std::shared_ptr<Item>>& items);
    const std::vector<std::shared_ptr<Item>>& getItems() const;
    
    void removeModifierFromHand(unsigned modifiersNum);
    void removeItemEquipped(unsigned itemsNum);

    std::shared_ptr<Modifier> popModifier(unsigned idx);
    const std::vector<std::shared_ptr<Modifier>>& getModifiers() const { return m_modifiers; }
    void addModifiers(const std::vector<std::shared_ptr<Modifier>>& modifiers) { m_modifiers = modifiers; }
    void addModifier(const std::shared_ptr<Modifier>& modifier);

private:
    uint m_level = 1;
    std::string m_name;

    std::vector<std::shared_ptr<Item>> m_items;
    std::vector<std::shared_ptr<Modifier>> m_modifiers;
};
