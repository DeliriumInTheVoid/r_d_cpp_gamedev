#pragma once

#include "network/commands.hpp"
#include "game/game_objects/game_object_frame_restorer.hpp"

namespace bt
{
    class game_object_frame_restorer_packet : public game_object_frame_restorer
    {
    public:
        explicit game_object_frame_restorer_packet(const std::weak_ptr<sf::Packet>& packet)
            : packet_{ packet }
        {
        }

        ~game_object_frame_restorer_packet() override = default;

    public:
        virtual void restore_frame(game_object_frame& frame) const override
        {
            if (packet_.expired())
            {
                return;
            }
            const auto packet = packet_.lock();
            frame.read_from_packet(*packet);
        }

    private:
        std::weak_ptr<sf::Packet> packet_;
    };
}
