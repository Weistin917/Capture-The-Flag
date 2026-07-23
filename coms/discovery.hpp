#pragma once
#include <string>

constexpr int DISCOVERY_PORT = 8888;      // fixed, shared by all class projects (protocol 1.2)
constexpr int PROTOCOL_VERSION = 1;
constexpr int DISCOVERY_TIMEOUT_MS = 3000; // how long client waits for server_info replies

// Server side: binds DISCOVERY_PORT, answers every "discover" with "server_info".
// Blocks forever (this is meant to run in its own thread later).
void run_server_discovery(const std::string& server_name, int tcp_port);

// Client side: broadcasts "discover", collects "server_info" replies until
// DISCOVERY_TIMEOUT_MS passes, then prints the list found.
void run_client_discovery();
