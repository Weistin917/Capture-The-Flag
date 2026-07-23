#include "tcp_client.hpp"

#include <nlohmann/json.hpp>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using json = nlohmann::json;

namespace {
constexpr int BUF_SIZE = 4096;

void handle_message(const json& msg, std::string& my_id) {
    if (!msg.contains("type")) return;
    std::string type = msg["type"];

    if (type == "welcome" && msg.contains("player_id")) {
        my_id = msg["player_id"];
        std::cout << "[Client] Assigned ID: " << my_id << "\n";
    }
    else if (type == "lobby" && msg.contains("players")) {
        std::cout << "\n=== Lobby ===\n";
        for (const auto& p : msg["players"]) {
            std::string pid = p.value("id", "?");
            std::string pname = p.value("name", "");
            std::cout << "  - " << (pname.empty() ? "(joining...)" : pname)
                      << " (" << pid << (pid == my_id ? ", you" : "") << ")\n";
        }
    }
    else if (type == "error" && msg.contains("reason")) {
        std::cout << "[Client] Error: " << msg["reason"].get<std::string>() << "\n";
    }
    // TODO: countdown / start / state / game_over once the game loop exists
}
} // namespace

void run_tcp_client(ServerEntry server, std::string player_name) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("[Client] socket"); return; }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(server.tcp_port);
    inet_pton(AF_INET, server.ip.c_str(), &addr.sin_addr);

    std::cout << "[Client] Connecting to " << server.ip << ":" << server.tcp_port << "...\n";
    if (connect(sockfd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("[Client] connect");
        close(sockfd);
        return;
    }

    json join;
    join["type"] = "join";
    join["v"] = PROTOCOL_VERSION;
    join["name"] = player_name;
    std::string out = join.dump() + "\n";
    send(sockfd, out.c_str(), out.size(), MSG_NOSIGNAL);

    std::string my_id;
    std::string buffer;
    char chunk[BUF_SIZE];

    while (true) {
        int bytes = recv(sockfd, chunk, sizeof(chunk) - 1, 0);
        if (bytes <= 0) {
            std::cout << "[Client] Disconnected from server\n";
            break;
        }
        chunk[bytes] = '\0';
        buffer += chunk;

        size_t pos;
        while ((pos = buffer.find('\n')) != std::string::npos) {
            std::string line = buffer.substr(0, pos);
            buffer.erase(0, pos + 1);
            if (line.empty()) continue;

            json msg;
            try {
                msg = json::parse(line);
            } catch (const json::parse_error&) {
                continue;
            }
            handle_message(msg, my_id);
        }
    }

    close(sockfd);
    // TODO: reconnect logic per requirements.md, once "server closed" vs
    // "we're leaving" can be told apart.
}
