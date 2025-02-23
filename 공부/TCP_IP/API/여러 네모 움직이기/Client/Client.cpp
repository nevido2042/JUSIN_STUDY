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

// 클라이언트 위치 정보를 저장할 구조체
struct ClientInfo {
    int x, y;
};

// 다른 클라이언트들의 위치를 저장하는 벡터
std::vector<ClientInfo> otherClients;

// 서버에 이동 명령 보내는 함수
void SendMoveCommand(char command) {
    send(clientSocket, &command, 1, 0);
}

// 사각형을 화면에 그리는 함수
void DrawRectangle(HDC hdc, int x, int y) {
    Rectangle(hdc, x, y, x + 30, y + 30);  // 30x30 크기의 사각형
}

// 다른 클라이언트들의 사각형을 그리는 함수
void DrawOtherClients(HDC hdc) {
    for (const auto& client : otherClients) {
        DrawRectangle(hdc, client.x, client.y);
    }
}

// 윈도우 프로시저 함수
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

        // 자기 자신과 다른 클라이언트들 그리기
        DrawRectangle(hdc, 100, 100);  // 자기 자신 (고정된 위치)
        DrawOtherClients(hdc);  // 다른 클라이언트들 그리기

        EndPaint(hwnd, &ps);
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// 서버에서 다른 클라이언트들의 위치 정보를 받는 함수
void ReceiveClientPositions() {
    ClientInfo clientInfo;
    while (true) {
        int bytesReceived = recv(clientSocket, (char*)&clientInfo, sizeof(ClientInfo), 0);
        if (bytesReceived > 0) {
            otherClients.push_back(clientInfo);  // 받은 클라이언트 정보 추가
            InvalidateRect(GetForegroundWindow(), NULL, TRUE);  // 화면 갱신
        }
    }
}

// WinMain 함수로 변경
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // WinSock 초기화
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // 클라이언트 소켓 생성
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;

    // inet_pton 사용하여 IP 주소 변환
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        std::cerr << "IP 주소 변환 실패!" << std::endl;
        return 1;
    }

    serverAddr.sin_port = htons(SERVER_PORT);


    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "서버 연결 실패!" << std::endl;
        return 1;
    }

    std::cout << "서버 연결 성공!" << std::endl;

    // 윈도우 클래스 설정
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"ClientWindow";

    RegisterClass(&wc);

    // 윈도우 생성
    HWND hwnd = CreateWindow(wc.lpszClassName, L"WinAPI Client",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 400,
        NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);

    // 서버에서 다른 클라이언트들의 위치 정보를 받는 스레드
    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReceiveClientPositions, NULL, 0, NULL);

    // 메시지 루프
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // 클라이언트 종료
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
