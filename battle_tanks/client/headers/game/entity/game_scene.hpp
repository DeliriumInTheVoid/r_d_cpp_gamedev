#pragma once

#include <memory>
#include <ranges>

namespace bt
{
    class game_scene
    {
    public:
        void update(const float delta_time) const
        {
            for (const auto& game_object : game_objects_ | std::views::values)
            {
                game_object->update(delta_time);
            }
        }

        void add_game_object(const std::shared_ptr<bt::game_object>& game_object)
        {
            game_objects_.insert({ game_object->get_id(), game_object });
        }

        void remove_game_object(const std::shared_ptr<bt::game_object>& game_object)
        {
            game_objects_.erase(game_object->get_id());
        }

        std::shared_ptr<bt::game_object> get_game_object(const bt::uuid id) const
        {
            return game_objects_.at(id);
        }

    private:
        std::unordered_map<bt::uuid, std::shared_ptr<bt::game_object>> game_objects_{};
    };
}
