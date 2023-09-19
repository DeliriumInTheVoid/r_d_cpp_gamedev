#pragma once

#include <memory>
#include <nlohmann/json.hpp>

class WeaponSkill;
class MonsterPolicy;
class ModifierDeck;
class ItemDeck;
class MonsterDeck;

class GameDataLoader
{
public:
    void loadMonsterDeck(MonsterDeck& deck, ItemDeck& itemsDeck, ModifierDeck& modifiersDeck);
    void loadItemsDeck(ItemDeck& deck) const;
    void loadModifiersDeck(ModifierDeck& deck) const;

private:
    std::shared_ptr<MonsterPolicy> createRunaway(const nlohmann::json& runaway_json) const;
    std::unique_ptr<WeaponSkill> createWeaponSkill(const nlohmann::json& skill_json) const;
    std::shared_ptr<MonsterPolicy> createVictory(const nlohmann::json& runawvictory_jsonay_json, ItemDeck& itemsDeck, ModifierDeck& modifierDeck);

};
