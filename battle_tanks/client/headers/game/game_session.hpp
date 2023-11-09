#pragma once


namespace bt
{
    class player_connecting_state;

    class game_session
    {
        friend class bt::player_connecting_state;
    public:
        game_session() = default;

        ~game_session() = default;

    public:
        bt::uuid get_player_id() const
        {
            return player_id_;
        }

        sf::Uint32 get_session_id() const
        {
            return session_id_;
        }

    private:
        bt::uuid player_id_{};
        sf::Uint32 session_id_{};
    };
}
