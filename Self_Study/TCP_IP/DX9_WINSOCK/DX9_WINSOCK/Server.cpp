#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_PORT 5000
#define MAX_CLIENTS 10

struct Client {
    SOCKET socket;
    sockaddr_in address;
};

std::vector<Client> clients;

void ProcessClientMessages(Client& client);

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr = { 0 };
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(SERVER_PORT);

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, MAX_CLIENTS);

    printf("Server listening on port %d...\n", SERVER_PORT);

    fd_set readSet;
    while (true) {
        FD_ZERO(&readSet);
        FD_SET(serverSocket, &readSet);

        for (auto& client : clients) {
            FD_SET(client.socket, &readSet);
        }

        select(0, &readSet, NULL, NULL, NULL);

        if (FD_ISSET(serverSocket, &readSet)) {
            sockaddr_in clientAddr;
            int addrLen = sizeof(clientAddr);
            SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &addrLen);

            printf("Client connected: %s\n", inet_ntoa(clientAddr.sin_addr));

            clients.push_back({ clientSocket, clientAddr });
        }

        for (auto& client : clients) {
            if (FD_ISSET(client.socket, &readSet)) {
                ProcessClientMessages(client);
            }
        }
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}

void ProcessClientMessages(Client& client) {
    char buffer[512];
    int bytesReceived = recv(client.socket, buffer, sizeof(buffer), 0);
    if (bytesReceived <= 0) {
        printf("Client disconnected\n");
        closesocket(client.socket);
        clients.erase(std::remove_if(clients.begin(), clients.end(), [&](const Client& c) {
            return c.socket == client.socket;
            }), clients.end());
        return;
    }

    buffer[bytesReceived] = '\0';
    printf("Received: %s\n", buffer);
}
