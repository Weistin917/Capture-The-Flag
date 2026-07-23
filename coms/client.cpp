#include "client.hpp"
#include "discovery.hpp"
#include <thread>
#include <iostream>

void client() {
    std::cout << "=== CTF Client ===\n";

    std::thread discover_thread(run_client_discovery);

    // TODO: read_user_input() loop here — let the player pick a server
    //       from the list (or enter an IP manually, protocol 1.3) and
    //       trigger connect_to_server(); once that exists, discovery
    //       should keep re-running in this thread until the user connects
    //       instead of a single pass, and player input (movement, etc.)
    //       will run concurrently with it.

    discover_thread.join();
}
