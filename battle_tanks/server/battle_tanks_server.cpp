#include "game_server.hpp"

int main()
{
    game_server server{};

    if (server.start(52000))
    {
        server.run_infinity_loop(100, 100, true);
    }

    return 0;
}
