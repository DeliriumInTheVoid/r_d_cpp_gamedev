#pragma once

#include "game/entity/game_object_type.hpp"

namespace bt
{
    struct game_object_b2d_link
    {
        game_object_type type{ game_object_type::unknown };
        bt::uuid id{ 0 };
    };
}
