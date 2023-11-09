#pragma once

#include <functional>
#include <iostream>
#include <ranges>

#include <SFML/Network.hpp>

#include "network/server_connection.hpp"
#include "network/commands.hpp"
#include "utils/uuid.hpp"

using client_command_handler = std::function<void(const sf::Uint32 connection_id, sf::Packet&)>;
//template<typename T>
//using client_command_handler = std::function<void(const sf::Uint32 connection_id, const sf::Uint32 session_id, const T& command)>;

class network_server
{
public:
    network_server(const unsigned short port) : port_{ port }
    {}

    ~network_server()
    {
        stop();
    }

    bool start();
    void stop();
    void update(size_t max_messages = -1, bool wait = false);

    void send_command(const sf::Uint32 connection_id, const std::unique_ptr<server_command>& command);
    void send_command(const std::vector<sf::Uint32>& connection_id, const std::unique_ptr<server_command>& command);

    void register_client_command_handler(const command_id_client command_id, const client_command_handler& handler);

private:
    void wait_for_client_connection();
    sf::Uint32 generate_client_id();
    void on_client_message(const sf::Uint32 connection_id, const std::shared_ptr<sf::Packet>& packet);
    void update_connection();

private:
    unsigned short port_{};
    bool is_running_{ false };

    std::thread thread_accept_awaiter_;
    std::thread thread_connections_worker_;
    std::mutex mutex_connections_;

    std::unique_ptr<sf::TcpListener> listener_{ nullptr };

    std::unordered_map<sf::Uint32, std::shared_ptr<server_connection>> connections_{};

    std::shared_ptr<ts_deque<connection_command_in>> commands_in_{ nullptr };

    std::unordered_map<command_id_client, client_command_handler> client_command_handlers_;

};

bool network_server::start()
{
    commands_in_ = std::make_shared<ts_deque<connection_command_in>>();
    listener_ = std::make_unique<sf::TcpListener>();

    if (listener_->listen(port_) != sf::Socket::Done)
    {
        return false;
    }

    is_running_ = true;
    thread_accept_awaiter_ = std::thread{ &network_server::wait_for_client_connection, this };
    thread_connections_worker_ = std::thread{ &network_server::update_connection, this };


    return true;
}

void network_server::stop()
{
    is_running_ = false;
    if (thread_accept_awaiter_.joinable())
    {
        thread_accept_awaiter_.join();
    }
    if (thread_connections_worker_.joinable())
    {
        thread_connections_worker_.join();
    }
}

void network_server::update(const size_t max_messages, const bool wait)
{
    if (wait)
    {
        commands_in_->wait();
    }

    size_t message_count = 0;
    while (message_count < max_messages && !commands_in_->empty())
    {
        auto [client_id, session_id, packet] = commands_in_->pop_front();

        on_client_message(client_id, packet);

        message_count++;
    }
}

void network_server::send_command(const sf::Uint32 connection_id, const std::unique_ptr<server_command>& command)
{
    auto packet = sf::Packet{};
    command->write_to_packet(packet);

    std::lock_guard lock(mutex_connections_);
    if (connections_.contains(connection_id))
    {
        connections_[connection_id]->send(packet);
    }
}

void network_server::send_command(const std::vector<sf::Uint32>& connection_id, const std::unique_ptr<server_command>& command)
{
    auto packet = sf::Packet{};
    command->write_to_packet(packet);

    std::lock_guard lock(mutex_connections_);
    for (const auto& id : connection_id)
    {
        if (connections_.contains(id))
        {
            connections_[id]->send(packet);
        }
    }
}

void network_server::register_client_command_handler(const command_id_client command_id, const client_command_handler& handler)
{
    client_command_handlers_[command_id] = handler;
}

void network_server::on_client_message(const sf::Uint32 connection_id, const std::shared_ptr<sf::Packet>& packet)
{
    sf::Uint32 rough_cmd_id;
    *packet >> rough_cmd_id;

    const auto command_id = static_cast<::command_id_client>(rough_cmd_id);

    if (client_command_handlers_.contains(command_id))
    {
        client_command_handlers_[command_id](connection_id, *packet);
    }
    else
    {
        std::cout << "Unregistered command handler for command id: " << rough_cmd_id
            << "; connection id: " << connection_id << std::endl;
    }
}

void network_server::update_connection()
{
    while (is_running_)
    {
        //TODO:: check connection without valuable packaged during some timeout and kill
        std::lock_guard lock(mutex_connections_);
        std::vector<sf::Uint32> connections_to_remove;
        for (const auto& connection : connections_ | std::views::values)
        {
            if (connection->update() == sf::Socket::Disconnected)
            {
                auto packet = sf::Packet{};
                const auto connection_id = connection->get_connection_id();
                const auto session_id = connection->get_session_id();

                packet << static_cast<sf::Uint32>(command_id_client::lost_connection);
                packet << connection_id;
                packet << session_id;
                connection_command_in command
                {
                    connection_id,
                    session_id,
                    std::make_shared<sf::Packet>(packet)
                };

                commands_in_->push_back(command);
                connections_to_remove.push_back(connection_id);
            }
        }

        for (const auto connection_id : connections_to_remove)
        {
            connections_.erase(connection_id);
        }
    }
}

void network_server::wait_for_client_connection()
{
    while (is_running_)
    {
        auto client_socket = std::make_unique<sf::TcpSocket>();
        listener_->accept(*client_socket);

        std::cout << "Client connected: " << client_socket->getRemoteAddress().toString() << std::endl;

        const auto client_connection = std::make_shared<server_connection>(std::move(client_socket), commands_in_);
        const sf::Uint32 connection_id = generate_client_id();

        client_connection->connect(connection_id);

        std::lock_guard lock(mutex_connections_);
        connections_.emplace(connection_id, client_connection);
    }
}

sf::Uint32 network_server::generate_client_id()
{
    return bt::generate_uuid();
}
