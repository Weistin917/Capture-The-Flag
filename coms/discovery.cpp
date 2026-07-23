#include "discovery.hpp"

#include <nlohmann/json.hpp>
#include <iostream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using json = nlohmann::json;

namespace {
constexpr int BUF_SIZE = 4096;
}

// ---------------------------------------------------------------------------
// SERVER SIDE
// ---------------------------------------------------------------------------
void run_server_discovery(const std::string& server_name, int tcp_port) {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) { perror("[Discovery] socket"); return; }

    int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(DISCOVERY_PORT);

    if (bind(sockfd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("[Discovery] bind");
        close(sockfd);
        return;
    }

    std::cout << "[Discovery] Listening on UDP " << DISCOVERY_PORT
              << " | name=\"" << server_name << "\" tcp_port=" << tcp_port << "\n";

    char buffer[BUF_SIZE];
    sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);

    while (true) {
        int bytes = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                              (sockaddr*)&client_addr, &client_len);
        if (bytes <= 0) continue;
        buffer[bytes] = '\0';

        json req;
        try {
            req = json::parse(buffer);
        } catch (const json::parse_error&) {
            continue; // discovery packets are best-effort, no error framing over UDP
        }

        if (!req.contains("type") || req["type"] != "discover") continue;

        json resp;
        resp["type"] = "server_info";
        resp["v"] = PROTOCOL_VERSION;
        resp["name"] = server_name;
        resp["tcp_port"] = tcp_port;
        resp["state"] = "lobby";  // TODO: pull from real lobby/game state
        resp["players"] = 0;      // TODO: pull from real player count

        std::string out = resp.dump();
        sendto(sockfd, out.c_str(), out.size(), 0,
               (sockaddr*)&client_addr, client_len);

        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));
        std::cout << "[Discovery] Answered discover from " << ip << "\n";
    }

    close(sockfd);
}

// ---------------------------------------------------------------------------
// CLIENT SIDE
// ---------------------------------------------------------------------------
namespace {
struct ServerEntry {
    std::string ip;
    std::string name;
    int tcp_port;
    std::string state;
    int players;
};
}

void run_client_discovery() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) { perror("[Discovery] socket"); return; }

    int broadcast_enable = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) < 0) {
        perror("[Discovery] SO_BROADCAST");
    }

    sockaddr_in bcast_addr{};
    bcast_addr.sin_family = AF_INET;
    bcast_addr.sin_port = htons(DISCOVERY_PORT);
    bcast_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST); // 255.255.255.255

    json req;
    req["type"] = "discover";
    req["v"] = PROTOCOL_VERSION;
    std::string out = req.dump();

    std::cout << "[Discovery] Broadcasting discover...\n";
    if (sendto(sockfd, out.c_str(), out.size(), 0,
               (sockaddr*)&bcast_addr, sizeof(bcast_addr)) < 0) {
        perror("[Discovery] sendto");
        close(sockfd);
        return;
    }

    timeval tv{};
    tv.tv_sec = DISCOVERY_TIMEOUT_MS / 1000;
    tv.tv_usec = (DISCOVERY_TIMEOUT_MS % 1000) * 1000;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    std::vector<ServerEntry> found;
    char buffer[BUF_SIZE];

    while (true) {
        sockaddr_in from{};
        socklen_t from_len = sizeof(from);
        int bytes = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                              (sockaddr*)&from, &from_len);
        if (bytes <= 0) break; // timeout hit, stop collecting

        buffer[bytes] = '\0';
        json resp;
        try {
            resp = json::parse(buffer);
        } catch (const json::parse_error&) {
            continue;
        }

        if (!resp.contains("type") || resp["type"] != "server_info") continue;

        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &from.sin_addr, ip, sizeof(ip));

        ServerEntry entry;
        entry.ip = ip;
        entry.name = resp.value("name", "unknown");
        entry.tcp_port = resp.value("tcp_port", 0);
        entry.state = resp.value("state", "unknown");
        entry.players = resp.value("players", 0);

        bool duplicate = false;
        for (const auto& e : found) {
            if (e.ip == entry.ip && e.tcp_port == entry.tcp_port) { duplicate = true; break; }
        }
        if (!duplicate) found.push_back(entry);
    }

    close(sockfd);

    if (found.empty()) {
        std::cout << "[Discovery] No servers found.\n";
        return;
    }

    std::cout << "\n=== Servers found ===\n";
    for (size_t i = 0; i < found.size(); i++) {
        const auto& e = found[i];
        std::cout << "  [" << i << "] " << e.name << "  " << e.ip << ":" << e.tcp_port
                   << "  (" << e.state << ", " << e.players << " players)\n";
    }
    // TODO: manual IP fallback entry (protocol 1.3 requires it, wiring it up
    // once we get to the actual TCP connect step).
}
