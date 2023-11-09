#include <cstdlib>
#include <cstring>

#include "game_server.hpp"

int main(int argc, char* argv[])
{
    unsigned int port = 52000;

    if (argc > 1) {
        if (strcmp(argv[1], "-port") == 0 && argc > 2) {
            port = atoi(argv[2]);
        }
    }

    game_server server{};

    if (server.start(port))
    {
        std::cout << "Server started on port " << port << std::endl;
        server.run_infinity_loop(100, 100, true);
    }

    return 0;
}
