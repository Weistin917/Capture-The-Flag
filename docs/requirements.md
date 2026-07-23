# Capture the flag requirements
## Description
The basic flow of the process is the following. When the server starts, it begins listening with TCP at a specific port for possible connection of clients. At the same time, it will listen at the networks broadcast with UDP to respond to server discoveries. For each client that connects, it will send them a welcome message, as well as notify each of the connected clients of the new list of connected clients. Throughout all this time, the server is able to see the state of the lobby (the information of the connected clients), and waits for the user (on the server's side) to start the game.  
On the other hand, the client starts by discovering the active servers by sending a UDP message to broadcast. They then receive the information of the active servers and can choose which one to connect to. After joining one server, they arrive to the lobby, where they can see the other connected players. They wait until server indicates the start of the game.  
Once the game starts, the players send inputs to the server and the server keeps updated the state of the game by notifying each player, until the game ends.  

---
The program needs to use threads to handle the concurrency of the happening of events. The server needs a TCP listening thread, a UDP broadcast listening thread, and a thread per client to handle each connection.  
The client needs a UDP discovery thread (to be able to keep on asking for active servers while being able to receive inputs from the player), a TCP thread to handle the connection (and in case of the server closing the connection, to try reconnecting).

## Brief view of the process
### Main
```
BEGIN
main():
    parseArgs()
    if (arg1 == server):
        server(args)
    else if (arg1 == client):
        client(args)
END
```
### Server
```
BEGIN
server():
    open_TCP_listening_thread()
    open_UDP_broadcast_thread()

    loop:
        handle_user_actions()
    until(user_starts_game())
    close_TCP_listening_thread()
    change_state("playing")

    notify_countdown()
    handle_game()

TCP_listening_thread():
    loop:
        listen()
        create_handle_client_thread()
    until(game_starts())

UDP_broadcast_thread():
    loop:
        listen()
        send_server_info()
    // doesn't close, only changes the current state that's being sent

TCP_client_thread():
    loop:
        wait_for_game_start()
        listen()
        send_input_to_server()
END
```
### Client
```
BEGIN
client():
    open_discover_thread()
    loop:
        read_user_input()
    until(connect_to_server())

    loop:
        send_inputs_to_server()

discover_thread():
    loop:
        discover()
        update_server_list()
    until(connect_to_server())

TCP_connection_thread():
    handles_communication()
    if (server.disconnected):
        reconnect()
END
```
## Tools
The program is to be implemented with C++. For communication, use BSD sockets. For graphics interface, use SFML. For JSON formatting, use nlohmann/json.  
## Structure
```
Capture-The-Flag/
├── coms/           ← Communication files (server, client)
├── game/           ← Game logic files
├── ui/             ← User graphic interface files
└── utils/          ← Other utility files
```