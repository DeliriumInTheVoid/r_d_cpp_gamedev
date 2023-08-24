#include <memory>
#include <fstream>

#include <nlohmann/json.hpp>

#include "GameDataLoader.h"

#include "Item.h"
#include "ItemDeck.h"
#include "Modifier.h"
#include "ModifierDeck.h"
#include "Monster.h"
#include "MonsterDeck.h"
#include "RunawayType.h"
#include "VictoryPolicy.h"
#include "VictoryPolicyType.h"

void GameDataLoader::loadMonsterDeck(MonsterDeck& deck, ItemDeck& itemsDeck, ModifierDeck& modifiersDeck)
{
    nlohmann::json monsters_json;
    std::ifstream monsters_file("game_data/monsters.json");
    monsters_file >> monsters_json;

    for (const auto& monster_json : monsters_json["monsters"])
    {
        std::shared_ptr<MonsterPolicy> runaway{ nullptr };
        std::shared_ptr<MonsterPolicy> victory{ nullptr };
        if (monster_json.contains("runaway"))
        {
            runaway = createRunaway(monster_json["runaway"]);
        }

        if (monster_json.contains("victory"))
        {
            victory = createVictory(monster_json["victory"], itemsDeck, modifiersDeck);
        }

        auto monster = std::make_shared<Monster>(
            monster_json["name"],
            monster_json["level"],
            getTribe(monster_json["tribe"]),
            std::move(runaway),
            std::move(victory)
        );
        deck.addMonster(std::move(monster));
    }
}

void GameDataLoader::loadItemsDeck(ItemDeck& deck) const
{
    nlohmann::json items_json;
    std::ifstream items_file("game_data/items.json");
    items_file >> items_json;

    for (const auto& item_json : items_json["items"])
    {
        std::unique_ptr<WeaponSkill> skill{ nullptr };
        if (item_json.contains("skill"))
        {
            skill = createWeaponSkill(item_json["skill"]);
        }

        auto item = std::make_shared<Weapon>(item_json["name"], item_json["power"], std::move(skill));

        deck.addItem(item);
    }
}

void GameDataLoader::loadModifiersDeck(ModifierDeck& deck) const
{
    nlohmann::json modifiers_json;
    std::ifstream modifiers_file("game_data/modifiers.json");
    modifiers_file >> modifiers_json;

    for (const auto& modifier_json : modifiers_json["modifiers"])
    {
        std::string type = modifier_json["type"];
        if (type == "power")
        {
            deck.addModifier(std::make_shared<Modifier_Power>(
                modifier_json["name"],
                getTribeModifierTarget(modifier_json["target"]),
                modifier_json["value"],
                getAbilityAction(modifier_json["action"])
            ));
        }
        else if (type == "level")
        {
            deck.addModifier(std::make_shared<Modifier_Level>(
                modifier_json["name"],
                getTribeModifierTarget(modifier_json["target"]),
                modifier_json["value"],
                getAbilityAction(modifier_json["action"])
            ));
        }
        else if (type == "level_by_tribe")
        {
            deck.addModifier(std::make_shared<Modifier_LevelByTribe>(
                modifier_json["name"],
                getTribeModifierTarget(modifier_json["target"]),
                modifier_json["value"],
                getAbilityAction(modifier_json["action"]),
                getTribe(modifier_json["tribe"])
            ));
        }
    }
}

std::shared_ptr<MonsterPolicy> GameDataLoader::createRunaway(const nlohmann::json& runaway_json) const
{
    std::shared_ptr<MonsterPolicy> runaway{ nullptr };

    const auto runawayType = getRunawayType(runaway_json["type"]);

    switch (runawayType)
    {
    case RunawayType::LevelDowngrade:
        runaway = std::make_shared<Runaway_LevelDowngrade>(runaway_json["value"]);
        break;
    case RunawayType::LevelDowngradeIf:
        runaway = std::make_shared<Runaway_LevelDowngradeIf>(runaway_json["value"], runaway_json["hero_level"]);
        break;
    case RunawayType::RemoveItems:
        runaway = std::make_shared<Runaway_ItemEquipedRemoval>(runaway_json["value"]);
        break;
    case RunawayType::RemoveModifiers:
        runaway = std::make_shared<Runaway_ModifierFromHandRemoval>(runaway_json["value"]);
        break;
    case RunawayType::None:
    default: ;
    }

    return runaway;
}

std::shared_ptr<MonsterPolicy> GameDataLoader::createVictory(const nlohmann::json& victory_json, ItemDeck& itemsDeck, ModifierDeck& modifierDeck)
{
    std::shared_ptr<MonsterPolicy> victory{ nullptr };

    const auto victoryType = getVictoryPolicy(victory_json["type"]);

    switch (victoryType)
    {
    case VictoryPolicyType::AddItems:
        victory = std::make_shared<VictoryPolicy_AddItems>(itemsDeck, victory_json["value"]);
        break;
    case VictoryPolicyType::AddModifiers:
        victory = std::make_shared<VictoryPolicy_AddModifiers>(modifierDeck, victory_json["value"]);
        break;
    case VictoryPolicyType::UpdateLevel:
        victory = std::make_shared<VictoryPolicy_AddLevel>(victory_json["value"]);
        break;
    case VictoryPolicyType::None:
    default:;
    }

    return victory;
}

std::unique_ptr<WeaponSkill> GameDataLoader::createWeaponSkill(const nlohmann::json& skill_json) const
{
    std::unique_ptr<WeaponSkill> skill{ nullptr };

    const std::string skillType{ skill_json["type"] };

    if (skillType == "power_booster")
    {
        skill = std::make_unique<WeaponSkillPowerBooster>(
            getAbilityAction(skill_json["action"]),
            skill_json["value"]
        );
    }
    else if (skillType == "tribe_power_booster")
    {
        skill = std::make_unique<WeaponSkillTribePowerBooster>(
            getTribe(skill_json["tribe"]),
            getAbilityAction(skill_json["action"]),
            skill_json["value"]
        );
    }
    else if(skillType == "kill_booster")
    {
        skill = std::make_unique<WeaponKillMonster>(skill_json["monster_name"]);
    }

    return std::move(skill);
}
