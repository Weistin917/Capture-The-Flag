#include "tcp_server.hpp"
#include "discovery.hpp" // PROTOCOL_VERSION

#include <nlohmann/json.hpp>
#include <iostream>
#include <vector>
#include <mutex>
#include <atomic>
#include <thread>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using json = nlohmann::json;

namespace {
constexpr int BUF_SIZE = 4096;
constexpr int MAX_NAME_LEN = 20;

struct Player {
    std::string id;
    std::string name; // empty until "join" is received
    int sockfd = -1;
    bool active = true;
};

std::mutex players_mtx;
std::vector<Player> players;

std::string generate_id() {
    static std::atomic<int> counter{0};
    return "player_" + std::to_string(++counter);
}

void send_line(int sockfd, const json& msg) {
    std::string out = msg.dump() + "\n";
    send(sockfd, out.c_str(), out.size(), MSG_NOSIGNAL);
}

json make_welcome(const std::string& player_id) {
    json resp;
    resp["type"] = "welcome";
    resp["v"] = PROTOCOL_VERSION;
    resp["player_id"] = player_id;
    resp["config"] = json::object(); // TODO: fill in game constants once defined
    return resp;
}

json make_lobby() {
    json resp;
    resp["type"] = "lobby";
    resp["v"] = PROTOCOL_VERSION;
    json arr = json::array();

    std::lock_guard<std::mutex> lock(players_mtx);
    for (const auto& p : players) {
        if (!p.active) continue;
        json pj;
        pj["id"] = p.id;
        pj["name"] = p.name;
        arr.push_back(pj);
    }
    resp["players"] = arr;
    return resp;
}

void broadcast_lobby() {
    json msg = make_lobby();
    std::lock_guard<std::mutex> lock(players_mtx);
    for (const auto& p : players) {
        if (p.active && p.sockfd >= 0) send_line(p.sockfd, msg);
    }
}

void remove_player(const std::string& player_id) {
    std::lock_guard<std::mutex> lock(players_mtx);
    for (auto& p : players) {
        if (p.id == player_id) {
            p.active = false;
            if (p.sockfd >= 0) close(p.sockfd);
            p.sockfd = -1;
            break;
        }
    }
}

void handle_client(int client_fd) {
    std::string player_id = generate_id();

    {
        std::lock_guard<std::mutex> lock(players_mtx);
        players.push_back({player_id, "", client_fd, true});
    }

    send_line(client_fd, make_welcome(player_id));
    broadcast_lobby(); // everyone sees the new (still-nameless) slot right away

    std::cout << "[TCP] Client connected, assigned " << player_id << "\n";

    std::string buffer;
    char chunk[BUF_SIZE];

    while (true) {
        int bytes = recv(client_fd, chunk, sizeof(chunk) - 1, 0);
        if (bytes <= 0) break;
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
                continue; // TODO: send back INVALID_JSON once error framing exists
            }
            if (!msg.contains("type")) continue;
            std::string type = msg["type"];

            if (type == "join" && msg.contains("name")) {
                std::string name = msg["name"];
                if (name.size() > MAX_NAME_LEN) name = name.substr(0, MAX_NAME_LEN);

                {
                    std::lock_guard<std::mutex> lock(players_mtx);
                    for (auto& p : players) {
                        if (p.id == player_id) { p.name = name; break; }
                    }
                }

                std::cout << "[TCP] " << player_id << " joined as \"" << name << "\"\n";
                broadcast_lobby();
            }
            // TODO: "input" / "interact" once the game loop exists
        }
    }

    remove_player(player_id);
    broadcast_lobby();
    std::cout << "[TCP] " << player_id << " disconnected\n";
}
} // namespace

void run_tcp_listener(int tcp_port) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("[TCP] socket"); return; }

    int reuse = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(tcp_port);

    if (bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("[TCP] bind"); close(server_fd); return;
    }
    if (listen(server_fd, 10) < 0) {
        perror("[TCP] listen"); close(server_fd); return;
    }

    std::cout << "[TCP] Listening on port " << tcp_port << "\n";

    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) { perror("[TCP] accept"); continue; }

        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip));
        std::cout << "[TCP] New connection from " << ip << "\n";

        std::thread(handle_client, client_fd).detach();
    }

    close(server_fd);
}
