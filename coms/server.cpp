#include "server.hpp"
#include "discovery.hpp"
#include <thread>
#include <iostream>

void server(const std::string& name, int tcp_port) {
    std::cout << "=== CTF Server ===\n";

    std::thread udp_thread(run_server_discovery, name, tcp_port);

    // TODO: open_TCP_listening_thread() — accepts clients, spawns a
    //       per-client thread each; lives in coms/tcp_server.cpp later.
    // TODO: handle_user_actions() loop here (e.g. "start") until the
    //       operator starts the game, then close the TCP listener and
    //       move to countdown/playing.

    udp_thread.join(); // discovery thread runs forever until process exit
}
