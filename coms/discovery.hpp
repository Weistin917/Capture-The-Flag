#pragma once
#include <string>
#include <vector>

constexpr int DISCOVERY_PORT = 8888;         // fixed, shared by all class projects (protocol 1.2)
constexpr int PROTOCOL_VERSION = 1;
constexpr int DISCOVERY_TIMEOUT_MS = 1000;   // how long one discovery round waits for replies
constexpr int DISCOVERY_INTERVAL_MS = 3000;  // gap between rounds on the client

struct ServerEntry {
    std::string ip;
    std::string name;
    int tcp_port;
    std::string state;
    int players;
};

// Server side: binds DISCOVERY_PORT, answers every "discover" with "server_info".
// Blocks forever (meant to run in its own thread, for the server's whole lifetime).
void run_server_discovery(const std::string& server_name, int tcp_port);

// Client side: broadcasts one "discover" and collects "server_info" replies
// for DISCOVERY_TIMEOUT_MS. Returns whatever came back (possibly empty).
// Meant to be called repeatedly on an interval by the caller.
std::vector<ServerEntry> discover_once();
