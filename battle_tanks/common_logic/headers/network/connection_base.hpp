#pragma once

#include <SFML/Network.hpp>

#include "network/commands.hpp"
#include "data_structures/ts_deque.hpp"


class connection_base
{
public:
    connection_base(std::unique_ptr<sf::TcpSocket>&& socket, const std::shared_ptr<ts_deque<connection_command_in>>& commands_in)
        : socket_{ std::move(socket) }, commands_in_{ commands_in }
    {
        socket_->setBlocking(false);
    }

    virtual ~connection_base() = default;

public:
    void set_session_id(const sf::Uint32 id);
    sf::Uint32 get_session_id();
    sf::Uint32 get_connection_id() const;

    //void connect(const sf::Uint32 id);
    void send(const sf::Packet& packet);
    sf::Socket::Status update();

protected:
    std::unique_ptr<sf::TcpSocket> socket_;
    ts_deque<std::unique_ptr<sf::Packet>> packages_out_;
    std::shared_ptr<ts_deque<connection_command_in>> commands_in_{ nullptr };
    sf::Uint32 connection_id_{};
    sf::Uint32 session_id_{};
};

void connection_base::set_session_id(const sf::Uint32 id)
{
    session_id_ = id;
}

sf::Uint32 connection_base::get_session_id()
{
    return session_id_;
}

sf::Uint32 connection_base::get_connection_id() const
{
    return connection_id_;
}

//void connection::connect(const sf::Uint32 id)
//{
//    connection_id_ = id;
//    sf::Packet packet{};
//    constexpr auto command_id = static_cast<sf::Uint32>(command_id_server::connected_to_game);
//    packet << command_id << connection_id_;
//    send(packet);
//}

void connection_base::send(const sf::Packet& packet)
{
    packages_out_.push_back(std::make_unique<sf::Packet>(packet));
}

sf::Socket::Status connection_base::update()
{
    //receive packets
    auto packet = sf::Packet{};
    auto status = socket_->receive(packet);
    if (status == sf::Socket::Status::Done)
    {
        const connection_command_in command{ connection_id_, session_id_, std::make_shared<sf::Packet>(packet) };
        commands_in_->push_back(command);
    }

    if (status == sf::Socket::Status::Disconnected)
    {
        return status;
    }


    //send packets
    if (packages_out_.empty())
    {
        return status;
    }

    do
    {
        const auto& packet_out = packages_out_.front();

        status = socket_->send(*packet_out);
        if (status == sf::Socket::Status::Done)
        {
            packages_out_.pop_front();
        }

        if (status == sf::Socket::Status::Disconnected || status == sf::Socket::NotReady)
        {
            break;
        }
    }
    while (status == sf::Socket::Status::Done && !packages_out_.empty());

    return status;
}
