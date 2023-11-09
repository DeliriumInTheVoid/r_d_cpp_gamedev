#pragma once

#include "network/commands.hpp"

namespace bt
{
    class game_object_frame_restorer
    {
    public:
        virtual ~game_object_frame_restorer() = default;
    public:
        virtual void restore_frame(game_object_frame& frame) const = 0;
    };
}
