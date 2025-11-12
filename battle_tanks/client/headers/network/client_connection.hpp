#pragma once

#include "network/commands.hpp"
#include "network/connection_base.hpp"


namespace bt
{
    class client_connection : public connection_base
    {
    public:
        client_connection(const std::shared_ptr<ts_deque<connection_command_in>>& commands_in)
            : connection_base{ std::make_unique<sf::TcpSocket>(), commands_in }
        {
        }

        virtual ~client_connection() override = default;

    public:
        void connect(const sf::IpAddress& address, const unsigned short port) const
        {
	        socket_->connect(address, port);
        }
    };
}
