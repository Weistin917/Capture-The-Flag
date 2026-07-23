#include "utils/args.hpp"
#include "coms/server.hpp"
#include "coms/client.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    Args args = parse_args(argc, argv);

    if (args.mode == Mode::INVALID) {
        std::cerr << "Usage: " << argv[0] << " server|client [-name \"...\"] [-p PORT]\n";
        return 1;
    }

    if (args.mode == Mode::SERVER) {
        server(args.name, args.port);
    } else {
        client(args.name);
    }

    return 0;
}
