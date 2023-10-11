#pragma once

#include <SFML/Network/Packet.hpp>

#include "network/client_connection.hpp"


namespace bt
{
    class connection_service
    {
    public:
        explicit connection_service(const std::shared_ptr<ts_deque<connection_command_in>>& commands_in)
            : connection_(commands_in)
        {
            
        }

        ~connection_service() = default;

    public:
        //TODO:: encapsulate it and add events/callback for incoming commands (commands_in)
        bt::client_connection& get_connection()
        {
            return connection_;
        }

        void send(const client_command& command)
        {
            sf::Packet packet;
            command.write_to_packet(packet);

            connection_.send(packet);
        }

    private:
        bt::client_connection connection_;
    };
}
