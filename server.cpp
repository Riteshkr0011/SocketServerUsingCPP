#include<iostream>
#include<WinSock2.h>
#include<ws2tcpip.h>
#include<tchar.h>
#include<thread>
#include<vector>
#include<string>
#include<algorithm>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

bool initialise() {
    WSADATA data;
    return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

void interactWithClient(SOCKET clientSocket, vector<SOCKET>& clients, string name) {
    cout << name << " connected" << endl;
    char buffer[4096];

    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

        if (bytesReceived <= 0) {
            cout << name << " disconnected" << endl;

            // Notify all clients that this client has left
            string leaveMessage = name + " has left the chat.";
            for (auto client : clients) {
                if (client != clientSocket) {
                    send(client, leaveMessage.c_str(), leaveMessage.length(), 0);
                }
            }

            break;
        }

        string message(buffer, bytesReceived);
        cout << name << ": " << message << endl;

        // Broadcast the received message to all clients except the sender
        for (auto client : clients) {
            if (client != clientSocket) {
                send(client, message.c_str(), message.length(), 0);
            }
        }
    }

    auto it = find(clients.begin(), clients.end(), clientSocket);
    if (it != clients.end()) {
        clients.erase(it);
    }

    closesocket(clientSocket);
}

int main() {
    if (!initialise()) {
        cout << "Winsock initialization failed!" << endl;
        return 1;
    }

    cout << "Server Program" << endl;

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == INVALID_SOCKET) {
        cout << "Socket creation failed" << endl;
        return 1;
    }

    int port = 3000;
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);

    if (InetPton(AF_INET, _T("0.0.0.0"), &serverAddress.sin_addr) != 1) {
        cout << "Setting address structure failed" << endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    if (bind(listenSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
        cout << "Bind failed" << endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        cout << "Listen failed" << endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    cout << "Server has started listening on port: " << port << endl;

    vector<SOCKET> clients;
    while (true) {
        SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            cout << "Invalid client socket" << endl;
            continue;
        }

        clients.push_back(clientSocket);

        // Receive the client's name at the beginning
        char nameBuffer[4096];
        int nameLength = recv(clientSocket, nameBuffer, sizeof(nameBuffer), 0);
        string name(nameBuffer, nameLength);

        // Notify other clients that a new user has joined
        string joinMessage = name + " has joined the chat.";
        for (auto client : clients) {
            if (client != clientSocket) {
                send(client, joinMessage.c_str(), joinMessage.length(), 0);
            }
        }

        thread t1(interactWithClient, clientSocket, ref(clients), name);
        t1.detach();
    }

    closesocket(listenSocket);
    WSACleanup();
    return 0;
}
