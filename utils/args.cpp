#include "args.hpp"
#include <cstdlib>

Args parse_args(int argc, char* argv[]) {
    Args args;

    if (argc < 2) return args; // INVALID, caller prints usage

    std::string mode_str = argv[1];
    if (mode_str == "server") {
        args.mode = Mode::SERVER;
        args.name = "CTF Server";
    } else if (mode_str == "client") {
        args.mode = Mode::CLIENT;
        args.name = "Player 1";
    } else {
        return args; // INVALID
    }

    for (int i = 2; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-name" && i + 1 < argc) {
            args.name = argv[++i];
        } else if (arg == "-p" && i + 1 < argc) {
            args.port = std::atoi(argv[++i]);
        }
    }

    return args;
}
