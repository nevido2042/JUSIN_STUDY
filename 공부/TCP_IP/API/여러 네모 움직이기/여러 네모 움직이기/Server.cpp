#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <iostream>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_PORT 5555

// 클라이언트 위치 정보를 저장할 구조체
struct ClientInfo {
    SOCKET socket;
    int x, y;
};

SOCKET serverSocket;
sockaddr_in serverAddr;

std::vector<ClientInfo> clients;  // 연결된 클라이언트들 저장

void HandleClient(SOCKET clientSocket) {
    char buffer[2];  // 'WASD' 명령
    int bytesReceived;

    while (true) {
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            char command = buffer[0];
            int clientIndex = -1;

            // 클라이언트 위치 업데이트
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

                // 다른 클라이언트들에게 위치 전송
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
        std::cerr << "소켓 생성 실패!" << std::endl;
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(SERVER_PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "바인드 실패!" << std::endl;
        return 1;
    }

    listen(serverSocket, 5);

    std::cout << "클라이언트 연결 대기 중...\n";

    while (true) {
        SOCKET clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket != INVALID_SOCKET) {
            std::cout << "클라이언트 연결됨\n";

            // 새 클라이언트를 clients 벡터에 추가
            clients.push_back({ clientSocket, 100, 100 });

            // 다른 클라이언트들에게 새 클라이언트 정보 전송
            for (const auto& client : clients) {
                if (client.socket != clientSocket) {
                    send(clientSocket, (char*)&client, sizeof(ClientInfo), 0);
                }
            }

            // 클라이언트를 위한 스레드 시작
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)HandleClient, (LPVOID)clientSocket, 0, NULL);
        }
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
