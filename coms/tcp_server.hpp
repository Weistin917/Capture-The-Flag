#pragma once

// Binds tcp_port, accepts connections forever, one thread per client.
// Sends welcome on connect, broadcasts the lobby list on any roster change.
// Meant to run in its own thread for the server's whole lifetime.
void run_tcp_listener(int tcp_port);
