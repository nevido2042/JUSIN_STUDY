#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <iostream>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_PORT 5555

// Ŭ���̾�Ʈ ��ġ ������ ������ ����ü
struct ClientInfo {
    SOCKET socket;
    int x, y;
};

SOCKET serverSocket;
sockaddr_in serverAddr;

std::vector<ClientInfo> clients;  // ����� Ŭ���̾�Ʈ�� ����

void HandleClient(SOCKET clientSocket) {
    char buffer[2];  // 'WASD' ���
    int bytesReceived;

    while (true) {
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            char command = buffer[0];
            int clientIndex = -1;

            // Ŭ���̾�Ʈ ��ġ ������Ʈ
            for (size_t i = 0; i < clients.size(); ++i) {
                if (clients[i].socket == clientSocket) {
                    clientIndex = i;
                    break;
                }
            }

            if (clientIndex != -1) {
                switch (command) {
                case 'W': clients[clientIndex].y -= 10; break;
                case 'S': clients[clientIndex].y += 10; break;
                case 'A': clients[clientIndex].x -= 10; break;
                case 'D': clients[clientIndex].x += 10; break;
                }

                // �ٸ� Ŭ���̾�Ʈ�鿡�� ��ġ ����
                for (size_t i = 0; i < clients.size(); ++i) {
                    if (clients[i].socket != clientSocket) {
                        send(clients[i].socket, (char*)&clients[clientIndex], sizeof(ClientInfo), 0);
                    }
                }
            }
        }
    }
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "���� ���� ����!" << std::endl;
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(SERVER_PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "���ε� ����!" << std::endl;
        return 1;
    }

    listen(serverSocket, 5);

    std::cout << "Ŭ���̾�Ʈ ���� ��� ��...\n";

    while (true) {
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket != INVALID_SOCKET) {
            std::cout << "Ŭ���̾�Ʈ �����\n";

            // �� Ŭ���̾�Ʈ�� clients ���Ϳ� �߰�
            clients.push_back({ clientSocket, 100, 100 });

            // �ٸ� Ŭ���̾�Ʈ�鿡�� �� Ŭ���̾�Ʈ ���� ����
            for (const auto& client : clients) {
                if (client.socket != clientSocket) {
                    send(clientSocket, (char*)&client, sizeof(ClientInfo), 0);
                }
            }

            // Ŭ���̾�Ʈ�� ���� ������ ����
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)HandleClient, (LPVOID)clientSocket, 0, NULL);
        }
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
