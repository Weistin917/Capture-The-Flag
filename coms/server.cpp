#include "server.hpp"
#include "discovery.hpp"
#include "tcp_server.hpp"
#include <thread>
#include <iostream>

void server(const std::string& name, int tcp_port) {
    std::cout << "=== CTF Server ===\n";

    std::thread udp_thread(run_server_discovery, name, tcp_port);
    std::thread tcp_thread(run_tcp_listener, tcp_port);

    // TODO: handle_user_actions() loop here (e.g. "start") until the
    //       operator starts the game, then move to countdown/playing.
    //       Discovery should keep answering the whole time (protocol 1.3),
    //       so it stays a plain join() here rather than a stop flag.

    udp_thread.join();
    tcp_thread.join();
}
