#pragma once

#include <SFML/Config.hpp>
#include <SFML/Network/Packet.hpp>
#include "SFML/System/Vector2.hpp"

#include "network/player_actions.hpp"
#include "game/entity/game_object_type.hpp"


enum class command_id_client : sf::Uint32
{
    unknown = 0,

    create_session = 90001,
    join_session = 90002,
    quit_session = 90003,

    player_action = 90004,

    lost_connection = 90005,
};

enum class command_id_server : sf::Uint32
{
    unknown = 0,

    connection_established = 10001,

    session_created = 10002,
    player_joined_to_session = 10003,
    player_quit_from_session = 10004,
    session_started = 10006,

    update_game_frame = 10007,

    delete_game_object = 10008,

    player_shoot = 10009,
};

template <class T>
struct simple_command
{
    explicit simple_command(const T id)
    {
        command_id = id;
    }
    virtual ~simple_command() = default;

    T command_id;

    virtual void write_to_packet(sf::Packet& packet) const
    {
        packet << static_cast<sf::Uint32>(command_id);
    }

    virtual void read_from_packet(sf::Packet& packet)
    {
        packet >> reinterpret_cast<sf::Uint32&>(command_id);
    }
};

////// TODO:: move from common to CLIENT
struct client_command : simple_command<command_id_client>
{
    explicit client_command(const command_id_client id) : simple_command{ id }
    {}

    virtual ~client_command() override = default;
};

struct join_session_command final : client_command
{
    explicit join_session_command(const sf::Uint32 session_id)
        : client_command(command_id_client::join_session), session_id{ session_id }
    {
    }
    virtual ~join_session_command() override = default;

    sf::Uint32 session_id;

    void write_to_packet(sf::Packet& packet) const override
    {
        simple_command::write_to_packet(packet);
        packet << session_id;
    }
};

struct player_action_command : client_command
{
    explicit player_action_command(const player_action action_type)
        : client_command(command_id_client::player_action)
    {
        action = action_type;
    }
    virtual ~player_action_command() override = default;

    player_action action;

    void write_to_packet(sf::Packet& packet) const override
    {
        simple_command::write_to_packet(packet);
        packet << static_cast<sf::Uint32>(action);
    }
};


////// TODO:: move to server_commands
struct server_command : simple_command<command_id_server>
{
    explicit server_command(const command_id_server id) : simple_command{ id }
    {
    }
    virtual ~server_command() override = default;
};

struct game_object_frame
{
    game_object_frame() = default;
    virtual ~game_object_frame() = default;

    sf::Uint32 game_object_id;
    bt::game_object_type game_object_type;
    sf::Vector2f position;
    float rotation;

    sf::Vector2f velocity;
    float velocity_angle;

    virtual void write_to_packet(sf::Packet& packet) const
    {
        packet << game_object_id;
        packet << static_cast<sf::Uint32>(game_object_type);
        packet << position.x;
        packet << position.y;
        packet << rotation;
        packet << velocity.x;
        packet << velocity.y;
        packet << velocity_angle;
    }

    virtual void read_from_packet(sf::Packet& packet)
    {
        //packet >> game_object_id;
        packet >> reinterpret_cast<sf::Uint32&>(game_object_type);
        packet >> position.x;
        packet >> position.y;
        packet >> rotation;
        packet >> velocity.x;
        packet >> velocity.y;
        packet >> velocity_angle;
    }
    
};

struct player_game_object_frame : game_object_frame
{
    virtual ~player_game_object_frame() override = default;

    float turret_rotation;
    sf::Int32 health;
    sf::Uint32 player_score;

    virtual void write_to_packet(sf::Packet& packet) const override
    {
        game_object_frame::write_to_packet(packet);
        packet << turret_rotation;
        packet << health;
        packet << player_score;
    }

    virtual void read_from_packet(sf::Packet& packet) override
    {
        game_object_frame::read_from_packet(packet);
        packet >> turret_rotation;
        packet >> health;
        packet >> player_score;
    }
};

struct game_frame_command final : server_command
{
    game_frame_command() : server_command(command_id_server::update_game_frame)
    {
    }
    virtual ~game_frame_command() override = default;

    std::vector<std::shared_ptr<game_object_frame>> game_objects;

    void write_to_packet(sf::Packet& packet) const override
    {
        simple_command::write_to_packet(packet);
        packet << static_cast<sf::Uint32>(game_objects.size());
        for (const auto& game_object : game_objects)
        {
            game_object->write_to_packet(packet);
        }
    }
};

struct deleted_game_object_command final : server_command
{
    explicit deleted_game_object_command(const sf::Uint32 game_object_id)
        : server_command(command_id_server::delete_game_object), game_object_id{ game_object_id }
    {
    }
    virtual ~deleted_game_object_command() override = default;

    sf::Uint32 game_object_id;

    void write_to_packet(sf::Packet& packet) const override
    {
        simple_command::write_to_packet(packet);
        packet << game_object_id;
    }
};

struct session_created_command : server_command
{
    session_created_command() : server_command(command_id_server::session_created)
    {
    }
    virtual ~session_created_command() override = default;

    sf::Uint32 session_id;

    void write_to_packet(sf::Packet& packet) const override
    {
        simple_command::write_to_packet(packet);
        packet << session_id;
    }
};

struct player_joined_to_session_command : server_command
{
    player_joined_to_session_command() : server_command(command_id_server::player_joined_to_session)
    {
    }
    virtual ~player_joined_to_session_command() override = default;

    sf::Uint32 session_id{};
    sf::Uint32 player_id{};

    void write_to_packet(sf::Packet& packet) const override
    {
        simple_command::write_to_packet(packet);
        packet << session_id;
        packet << player_id;
    }
};

struct session_started_command : server_command
{
    session_started_command() : server_command(command_id_server::session_started)
    {
    }
    virtual ~session_started_command() override = default;

    sf::Uint32 session_id{};
    std::vector<std::shared_ptr<game_object_frame>> game_objects;

    void write_to_packet(sf::Packet& packet) const override
    {
        simple_command::write_to_packet(packet);
        packet << session_id;
        packet << static_cast<sf::Uint32>(game_objects.size());
        for (const auto& game_object : game_objects)
        {
            game_object->write_to_packet(packet);
        }
    }
};

struct player_shoot final : server_command
{
    player_shoot() : server_command(command_id_server::player_shoot)
    {
    }
    virtual ~player_shoot() override = default;

    sf::Uint32 player_id{};
    game_object_frame game_object{};

    void write_to_packet(sf::Packet& packet) const override
    {
        simple_command::write_to_packet(packet);
        packet << player_id;
		game_object.write_to_packet(packet);
    }

    virtual void read_from_packet(sf::Packet& packet) override
    {
        //simple_command::read_from_packet(packet);
        packet >> player_id;
		game_object.read_from_packet(packet);
    }
};

///////////////////////////////////
struct connection_command_in
{
    sf::Uint32 connection_id;
    //TODO:: remove
    sf::Uint32 session_id;
    std::shared_ptr<sf::Packet> packet;
};

struct connection_command_out
{
    std::vector<sf::Uint32> connection_id;
    std::unique_ptr<server_command> command;
};

