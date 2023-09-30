#pragma once

#include "network/commands.hpp"
#include "network/connection_base.hpp"


class server_connection : public connection_base
{
public:
    server_connection(std::unique_ptr<sf::TcpSocket>&& socket, const std::shared_ptr<ts_deque<connection_command_in>>& commands_in)
        : connection_base{ std::move(socket) , commands_in }
    {
    }

    virtual ~server_connection() override = default;

public:
    void connect(const sf::Uint32 id);
};

void server_connection::connect(const sf::Uint32 id)
{
    connection_id_ = id;
    sf::Packet packet{};
    constexpr auto command_id = static_cast<sf::Uint32>(command_id_server::connected_to_game);
    packet << command_id << connection_id_;
    send(packet);
}
