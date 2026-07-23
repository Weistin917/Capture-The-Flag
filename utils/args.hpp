#pragma once
#include <string>

enum class Mode { SERVER, CLIENT, INVALID };

struct Args {
    Mode mode = Mode::INVALID;
    std::string name;
    int port = 3149; // TCP game port (server only, for now)
};

Args parse_args(int argc, char* argv[]);
