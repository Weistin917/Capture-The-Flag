#pragma once
#include <string>
#include "discovery.hpp" // ServerEntry

// Connects to the chosen server, sends "join" with player_name, then
// listens for server messages (welcome, lobby, ...) and prints updates.
// Blocks until disconnected. Meant to run in its own thread.
void run_tcp_client(ServerEntry server, std::string player_name);
