#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5555

SOCKET clientSocket;
sockaddr_in serverAddr;

void SendMoveCommand(char command) {
    send(clientSocket, &command, 1, 0);
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(SERVER_PORT);

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cout << "서버 연결 실패!\n";
        return 1;
    }

    std::cout << "서버 연결 성공!\n";
    std::cout << "WASD로 움직이세요!\n";

    while (true) {
        if (GetAsyncKeyState('W') & 0x8000) SendMoveCommand('W');
        if (GetAsyncKeyState('S') & 0x8000) SendMoveCommand('S');
        if (GetAsyncKeyState('A') & 0x8000) SendMoveCommand('A');
        if (GetAsyncKeyState('D') & 0x8000) SendMoveCommand('D');
        Sleep(50);
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
