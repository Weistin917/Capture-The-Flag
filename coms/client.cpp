#include "client.hpp"
#include "discovery.hpp"
#include "tcp_client.hpp"
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <iostream>
#include <chrono>

namespace {
std::mutex servers_mtx;
std::vector<ServerEntry> servers;
std::atomic<bool> discovering{true};

void print_servers() {
    std::lock_guard<std::mutex> lock(servers_mtx);
    std::cout << "\n=== Servers found ===\n";
    if (servers.empty()) {
        std::cout << "  (none yet)\n";
    }
    for (size_t i = 0; i < servers.size(); i++) {
        const auto& e = servers[i];
        std::cout << "  [" << i << "] " << e.name << "  " << e.ip << ":" << e.tcp_port
                   << "  (" << e.state << ", " << e.players << " players)\n";
    }
    std::cout << "Enter a number to connect (list refreshes every "
              << DISCOVERY_INTERVAL_MS / 1000 << "s): " << std::flush;
}

// Runs on its own thread: keeps discovering at an interval so the player can
// keep watching the list grow while typing their choice, per requirements.md.
void discovery_loop() {
    while (discovering) {
        auto found = discover_once();

        {
            std::lock_guard<std::mutex> lock(servers_mtx);
            for (auto& e : found) {
                bool dup = false;
                for (auto& existing : servers) {
                    if (existing.ip == e.ip && existing.tcp_port == e.tcp_port) {
                        existing = e; // refresh state/players
                        dup = true;
                        break;
                    }
                }
                if (!dup) servers.push_back(e);
            }
        }

        if (discovering) print_servers();
        std::this_thread::sleep_for(std::chrono::milliseconds(DISCOVERY_INTERVAL_MS));
    }
}
} // namespace

void client(const std::string& player_name) {
    std::cout << "=== CTF Client ===\n";

    std::thread discover_thread(discovery_loop);

    ServerEntry chosen;
    bool got_choice = false;
    std::string line;

    while (!got_choice && std::getline(std::cin, line)) {
        try {
            int idx = std::stoi(line);
            std::lock_guard<std::mutex> lock(servers_mtx);
            if (idx >= 0 && idx < (int)servers.size()) {
                chosen = servers[idx];
                got_choice = true;
            } else {
                std::cout << "[Client] Invalid index, try again.\n";
            }
        } catch (...) {
            std::cout << "[Client] Enter a valid number.\n";
        }
    }

    discovering = false;   // UDP discovery thread stops once we've chosen
    discover_thread.join();

    if (!got_choice) return; // stdin closed before a choice was made

    std::thread tcp_thread(run_tcp_client, chosen, player_name);
    tcp_thread.join();
}
