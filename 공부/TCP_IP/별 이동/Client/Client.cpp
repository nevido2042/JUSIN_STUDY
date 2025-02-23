#pragma comment(lib, "ws2_32")
#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <locale>

#define IP L"127.0.0.1"
#define PORT L"3000"
#define WIDTH 80
#define HEIGHT 23

enum MSG_TYPE {
    ALLOC_ID, CREATE_STAR, DELETE_STAR, MOVE_STAR
};

struct CLIENT {
    int x = 0;
    int y = 0;
    int id = 0;
};

struct MSG_ALLOC_ID {
    int type = ALLOC_ID;
    int id = 0;
    int zero[2] = { 0, };
};

struct MSG_CREATE_STAR {
    int type = CREATE_STAR;
    int id = 0;
    int x = 0;
    int y = 0;
};

struct MSG_DELETE_STAR {
    int type = DELETE_STAR;
    int id = 0;
    int zero[2] = { 0, };
};

struct MSG_MOVE_STAR {
    int type = MOVE_STAR;
    int id = 0;
    int x = 0;
    int y = 0;
};

// Global variables
char msg[16] = { 0, };
fd_set readSet;
fd_set tempSet;
timeval timeout;
SOCKET sock;
int myID;
CLIENT clientArr[30];
int clientCnt = 0;
wchar_t printArr[HEIGHT][WIDTH];

void Network();
void Input();
void Render();
void cs_Initial();
void printArrSet();

int main() {
    // 커서 제거
    cs_Initial();
    // 출력 배열 초기화
    printArrSet();

    setlocale(LC_ALL, "KOREAN");

    // 윈속 초기화
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        wprintf_s(L"WSAStartup() error:%d\n", WSAGetLastError());
        return 1;
    }

    // 소켓 생성
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        wprintf_s(L"socket() error:%d\n", WSAGetLastError());
        return 1;
    }

    // 논블로킹 모드로 전환
    u_long mode = 1;
    ioctlsocket(sock, FIONBIO, &mode);

    // 주소 설정
    SOCKADDR_IN adr;
    ZeroMemory(&adr, sizeof(adr));
    adr.sin_family = AF_INET;
    InetPton(AF_INET, (PCWSTR)IP, &adr.sin_addr.s_addr);
    adr.sin_port = htons(_wtoi(PORT));

    // 서버에 연결
    if (connect(sock, (SOCKADDR*)&adr, sizeof(adr)) == SOCKET_ERROR) {
        wprintf_s(L"connect() error:%d\n", WSAGetLastError());
    }

    FD_ZERO(&readSet);
    FD_SET(sock, &readSet);

    // 클라이언트 배열 초기화
    ZeroMemory(clientArr, sizeof(clientArr));

    // 메인 루프
    while (1) {
        Input();
        Network();
        Render();
        system("cls");
    }

    return 0;
}

void cs_Initial() {
    CONSOLE_CURSOR_INFO stConsoleCursor;
    stConsoleCursor.bVisible = FALSE;
    stConsoleCursor.dwSize = 1;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorInfo(hConsole, &stConsoleCursor);
}

void printArrSet() {
    // 출력 배열 초기화
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printArr[i][j] = (wchar_t)L' ';
            if (j == WIDTH - 1)
                printArr[i][j] = (wchar_t)L'\0';
        }
    }
}

void Render() {
    wprintf_s(L"클라이언트 수: %d\n", clientCnt);

    printArrSet();

    for (int h = 0; h < clientCnt; h++) {
        printArr[clientArr[h].y][clientArr[h].x] = (wchar_t)L'*';
    }

    for (int i = 0; i < HEIGHT; i++) {
        wprintf_s(L"%s", printArr[i]);
        _putws(L"");
    }
}

void Input() {
    CLIENT* myClient = NULL;
    for (int i = 0; i < clientCnt; i++) {
        if (clientArr[i].id == myID) {
            myClient = &clientArr[i];
            break;
        }
    }

    if (myClient == NULL) return;

    CLIENT tempClient = *myClient;

    if (GetAsyncKeyState(VK_LEFT)) tempClient.x--;
    if (GetAsyncKeyState(VK_RIGHT)) tempClient.x++;
    if (GetAsyncKeyState(VK_UP)) tempClient.y--;
    if (GetAsyncKeyState(VK_DOWN)) tempClient.y++;

    // 좌표값 제한
    if (tempClient.x > WIDTH - 1) tempClient.x = WIDTH - 1;
    if (tempClient.x < 0) tempClient.x = 0;

    if (tempClient.y > HEIGHT - 1) tempClient.y = HEIGHT - 1;
    if (tempClient.y < 0) tempClient.y = 0;

    // 좌표값이 바뀌었을 때 전송
    if (myClient->x != tempClient.x || myClient->y != tempClient.y) {
        MSG_MOVE_STAR msgMoveStar;
        msgMoveStar.id = tempClient.id;
        msgMoveStar.x = tempClient.x;
        msgMoveStar.y = tempClient.y;

        if (send(sock, (char*)&msgMoveStar, sizeof(msgMoveStar), 0) == SOCKET_ERROR) {
            wprintf_s(L"send() error:%d", WSAGetLastError());
        }

        // 바뀐값 클라이언트에 적용
        *myClient = tempClient;
    }
}

void Network() {
    while (1) {
        tempSet = readSet;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        int retSelect = select(0, &tempSet, 0, 0, &timeout);

        if (WSAGetLastError() == WSAEWOULDBLOCK || retSelect == 0) break;

        if (retSelect == SOCKET_ERROR) {
            wprintf_s(L"select() error:%d\n", WSAGetLastError());
            break;
        }

        if (FD_ISSET(sock, &tempSet)) {
            int retRecv = recv(sock, msg, sizeof(msg), 0);
            if (retRecv == SOCKET_ERROR) {
                wprintf_s(L"recv() error:%d\n", WSAGetLastError());
                break;
            }

            int type = *(int*)msg;
            wprintf(L"받은 메시지 타입: %d\n", type);

            switch (type) {
            case ALLOC_ID: {
                MSG_ALLOC_ID* msgAllocID = (MSG_ALLOC_ID*)msg;
                myID = msgAllocID->id;
                wprintf_s(L"아이디는 %d\n", msgAllocID->id);
                break;
            }
            case CREATE_STAR: {
                MSG_CREATE_STAR* msgCreateStar = (MSG_CREATE_STAR*)msg;
                clientArr[clientCnt].id = msgCreateStar->id;
                clientArr[clientCnt].x = msgCreateStar->x;
                clientArr[clientCnt].y = msgCreateStar->y;
                clientCnt++;
                break;
            }
            case MOVE_STAR: {
                MSG_MOVE_STAR* msgMoveStar = (MSG_MOVE_STAR*)msg;
                for (int k = 0; k < clientCnt; k++) {
                    if (msgMoveStar->id == clientArr[k].id) {
                        clientArr[k].x = msgMoveStar->x;
                        clientArr[k].y = msgMoveStar->y;
                        break;
                    }
                }
                break;
            }
            case DELETE_STAR: {
                MSG_DELETE_STAR* msgDeleteStar = (MSG_DELETE_STAR*)msg;
                for (int i = 0; i < clientCnt; i++) {
                    if (msgDeleteStar->id == clientArr[i].id) {
                        clientArr[i] = clientArr[clientCnt - 1];
                        clientCnt--;
                        break;
                    }
                }
                break;
            }
            default: {
                wprintf(L"Unknown msg type: %d\n", type);
                closesocket(sock);
                WSACleanup();
                return;
            }
            }
        }
    }
}