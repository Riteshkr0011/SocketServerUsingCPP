# C++ Chat Application

This is a simple chat application implemented in C++ using WinSock2. It allows multiple clients to connect to a server and exchange messages in real time. The application features a basic chat system with colored terminal output and a custom exit mechanism where users can quit the chat by typing `quit`.

## Features

- **Multi-client support**: Multiple clients can connect to the server and chat with each other.
- **Real-time messaging**: Messages are sent and received in real time.
- **Username display**: Usernames are displayed with each sent message.
- **Colored terminal output**: Messages are color-coded for better readability.
- **Custom exit**: Clients can quit the chat by typing `quit`, notifying other users that they have left the chat.

## Requirements

- Windows OS (required for WinSock2)
- C++11 or later
- WinSock2 library

## Getting Started

### Prerequisites

Ensure you have the following installed:

- A C++ compiler (e.g., MinGW, MSVC)
- WinSock2 library (usually included with Windows development environments)

### Installation

1. **Clone the repository**:
    ```bash
    git clone https://github.com/yourusername/cpp-chat-application.git
    cd cpp-chat-application
    ```

2. **Compile the server**:
    ```bash
    g++ server.cpp -o server -lws2_32
    ```

3. **Compile the client**:
    ```bash
    g++ client.cpp -o client -lws2_32
    ```

### Running the Application

1. **Start the server**:
   - Run the compiled `server` program.
   - The server will start listening on `localhost:3000` for incoming client connections.

    ```bash
    ./server
    ```

2. **Start a client**:
   - Run the compiled `client` program.
   - Enter your username when prompted.
   - You can start sending messages after connecting to the server.

    ```bash
    ./client
    ```

3. **Join the chat**:
   - Multiple clients can connect to the server by running additional instances of the client program.

4. **Exit the chat**:
   - Type `quit` to quit the chat. This will notify other users that you have left the chat.

## Code Overview

### `server.cpp`

- Initializes a server socket using WinSock2.
- Listens for incoming client connections.
- Spawns a new thread for each connected client to handle message exchange.
- Broadcasts received messages to all connected clients.
- Notifies all clients when a user disconnects.

### `client.cpp`

- Connects to the server using WinSock2.
- Prompts the user to enter a username.
- Handles sending and receiving messages in separate threads.
- Detects `quit` to gracefully exit the chat and notify other users.

## Customization

- **Port Number**: Change the `port` variable in both `server.cpp` and `client.cpp` if you wish to use a different port.
- **Server Address**: Modify the `serverAddress` variable in `client.cpp` to connect to a server running on a different machine.
- **Message Colors**: You can customize the message colors by modifying the escape sequences in the `cout` statements.

## Troubleshooting

- **Winsock Initialization Failed**: Ensure that WinSock2 is correctly installed and linked in your development environment.
- **Connection Issues**: Verify that the server is running and accessible at the specified IP address and port.
- **Disconnected Clients**: If a client disconnects unexpectedly, the server will notify remaining clients. Ensure your network connection is stable.
