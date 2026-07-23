#pragma once
#include <string>

// Spawns and owns all server-side threads (UDP discovery for now,
// TCP listener + per-client threads later). Blocks until shutdown.
void server(const std::string& name, int tcp_port);
