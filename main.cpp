#include "utils/args.hpp"
#include "coms/discovery.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    Args args = parse_args(argc, argv);

    if (args.mode == Mode::INVALID) {
        std::cerr << "Usage: " << argv[0] << " server|client [-name \"...\"] [-p PORT]\n";
        return 1;
    }

    if (args.mode == Mode::SERVER) {
        run_server_discovery(args.name, args.port);
    } else {
        run_client_discovery();
    }

    return 0;
}
