#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <ws2tcpip.h> 

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5555

SOCKET clientSocket;
sockaddr_in serverAddr;

// Ŭ���̾�Ʈ ��ġ ������ ������ ����ü
struct ClientInfo {
    int x, y;
};

// �ٸ� Ŭ���̾�Ʈ���� ��ġ�� �����ϴ� ����
std::vector<ClientInfo> otherClients;

// ������ �̵� ��� ������ �Լ�
void SendMoveCommand(char command) {
    send(clientSocket, &command, 1, 0);
}

// �簢���� ȭ�鿡 �׸��� �Լ�
void DrawRectangle(HDC hdc, int x, int y) {
    Rectangle(hdc, x, y, x + 30, y + 30);  // 30x30 ũ���� �簢��
}

// �ٸ� Ŭ���̾�Ʈ���� �簢���� �׸��� �Լ�
void DrawOtherClients(HDC hdc) {
    for (const auto& client : otherClients) {
        DrawRectangle(hdc, client.x, client.y);
    }
}

// ������ ���ν��� �Լ�
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_KEYDOWN) {
        char command = 0;
        switch (wParam) {
        case 'W': command = 'W'; break;
        case 'S': command = 'S'; break;
        case 'A': command = 'A'; break;
        case 'D': command = 'D'; break;
        }
        if (command != 0) {
            SendMoveCommand(command);
        }
    }

    if (msg == WM_PAINT) {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // �ڱ� �ڽŰ� �ٸ� Ŭ���̾�Ʈ�� �׸���
        DrawRectangle(hdc, 100, 100);  // �ڱ� �ڽ� (������ ��ġ)
        DrawOtherClients(hdc);  // �ٸ� Ŭ���̾�Ʈ�� �׸���

        EndPaint(hwnd, &ps);
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// �������� �ٸ� Ŭ���̾�Ʈ���� ��ġ ������ �޴� �Լ�
void ReceiveClientPositions() {
    ClientInfo clientInfo;
    while (true) {
        int bytesReceived = recv(clientSocket, (char*)&clientInfo, sizeof(ClientInfo), 0);
        if (bytesReceived > 0) {
            otherClients.push_back(clientInfo);  // ���� Ŭ���̾�Ʈ ���� �߰�
            InvalidateRect(GetForegroundWindow(), NULL, TRUE);  // ȭ�� ����
        }
    }
}

// WinMain �Լ��� ����
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // WinSock �ʱ�ȭ
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Ŭ���̾�Ʈ ���� ����
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;

    // inet_pton ����Ͽ� IP �ּ� ��ȯ
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        std::cerr << "IP �ּ� ��ȯ ����!" << std::endl;
        return 1;
    }

    serverAddr.sin_port = htons(SERVER_PORT);


    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "���� ���� ����!" << std::endl;
        return 1;
    }

    std::cout << "���� ���� ����!" << std::endl;

    // ������ Ŭ���� ����
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"ClientWindow";

    RegisterClass(&wc);

    // ������ ����
    HWND hwnd = CreateWindow(wc.lpszClassName, L"WinAPI Client",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 400,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);

    // �������� �ٸ� Ŭ���̾�Ʈ���� ��ġ ������ �޴� ������
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReceiveClientPositions, NULL, 0, NULL);

    // �޽��� ����
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Ŭ���̾�Ʈ ����
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
