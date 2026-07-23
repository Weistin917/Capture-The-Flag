#pragma once
#include <string>

// Spawns and owns all client-side threads: repeating UDP discovery until
// the player picks a server, then the TCP connection thread. Blocks until
// the TCP session ends.
void client(const std::string& player_name);
