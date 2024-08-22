#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<thread>
#include<string>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

bool initialise() {
    WSADATA data;
    return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

void sendMessage(SOCKET s, const string& name) {
    string message;
    while (true) {
        getline(cin, message);
        string msg = name + ": " + message;  // Combine username and message
        int bytesSent = send(s, msg.c_str(), msg.length(), 0);

        if (bytesSent == SOCKET_ERROR) {
            cout << "\033[31m[Error] Sending message failed\033[0m" << endl;  // Red error message
            break;
        }
        if (message == "quit") {
            cout << "\033[31m[System] Stopping the Application\033[0m" << endl;  // Red system message
            break;
        }
        cout << "\033[35mYou: " << message << "\033[0m" << endl;  // Magenta "You:" with the message
    }
    closesocket(s);
    WSACleanup();
}

void receiveMessage(SOCKET s) {
    char buffer[4096];
    int receivedLength;
    string msg;
    while (true) {
        receivedLength = recv(s, buffer, sizeof(buffer), 0);
        if (receivedLength <= 0) {
            cout << "\033[31m[System] Disconnected from the server\033[0m" << endl;  // Red system message
            break;
        }
        else {
            msg = string(buffer, receivedLength);
            cout << "\033[33m" << msg << "\033[0m" << endl;  // Yellow for incoming messages
        }
    }
    closesocket(s);
    WSACleanup();
}

int main() {
    if (!initialise()) {
        cout << "\033[31m[Error] Winsock initialization failed\033[0m" << endl;  // Red error message
        return 1;
    }

    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        cout << "\033[31m[Error] Invalid socket created\033[0m" << endl;  // Red error message
        return 1;
    }

    int port = 3000;
    string serverAddress = "127.0.0.1";

    sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);

    inet_pton(AF_INET, serverAddress.c_str(), &(serveraddr.sin_addr));

    if (connect(s, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR) {
        cout << "\033[31m[Error] Server connection failed\033[0m" << endl;  // Red error message
        cout << ": " << WSAGetLastError();
        closesocket(s);
        WSACleanup();
        return 1;
    }
    cout << "\033[32m[System] Successfully connected to server\033[0m" << endl;  // Green system message

    cout << "\033[36mEnter your Chat Name: \033[0m";  // Cyan prompt for entering name
    string name;
    getline(cin, name);

    thread senderThread(sendMessage, s, name);
    thread receiverThread(receiveMessage, s);

    senderThread.join();
    receiverThread.join();
    return 0;
}
