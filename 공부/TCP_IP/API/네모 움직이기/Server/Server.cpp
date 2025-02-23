#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_PORT 5555
#define WIDTH  800
#define HEIGHT 600

// �簢�� ��ǥ
RECT rect = { 100, 100, 150, 150 };
int moveStep = 10;

// ������ ���ν��� ����
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// ���� �ʱ�ȭ
SOCKET serverSocket, clientSocket;
sockaddr_in serverAddr, clientAddr;
int clientLen = sizeof(clientAddr);

void MoveRectangle(char direction) {
    switch (direction) {
    case 'W': OffsetRect(&rect, 0, -moveStep); break;
    case 'S': OffsetRect(&rect, 0, moveStep); break;
    case 'A': OffsetRect(&rect, -moveStep, 0); break;
    case 'D': OffsetRect(&rect, moveStep, 0); break;
    }
}

// ���� ������
DWORD WINAPI ServerThread(LPVOID lpParam) {
    char buffer[2];
    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived > 0) {
            MoveRectangle(buffer[0]);
            InvalidateRect((HWND)lpParam, NULL, TRUE);
        }
    }
    return 0;
}

// WinMain �Լ�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"WinSockRectangleServer";
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(wc.lpszClassName, L"WinSock Rectangle Server",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // WinSock �ʱ�ȭ
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(SERVER_PORT);

    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 1);

    std::cout << "Ŭ���̾�Ʈ ��� ��...\n";
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
    std::cout << "Ŭ���̾�Ʈ �����!\n";

    // ���� ������ ����
    CreateThread(NULL, 0, ServerThread, hwnd, 0, NULL);

    // �޽��� ����
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    closesocket(serverSocket);
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}

// ������ ���ν���
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &rect, (HBRUSH)GetStockObject(GRAY_BRUSH));
        EndPaint(hwnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
