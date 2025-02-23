#pragma comment(lib, "ws2_32")
#include <stdio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <locale>

#define PORT L"3000"
#define WIDTH 81
#define HEIGHT 24
#define INIT_X 40
#define INIT_Y 12

enum MSG_TYPE {
    ALLOC_ID, CREATE_STAR, DELETE_STAR, MOVE_STAR
};

struct CLIENT {
    int x = 0;
    int y = 0;
    int id = 0;
};

struct MSG_BASE {
    int zero[4] = { 0, };
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

struct PlayerSession {
    SOCKADDR_IN clntAdr;
    SOCKET clntSock = INVALID_SOCKET;
    int id = 0;
    int x = 0;
    int y = 0;
};

SOCKET servSock = INVALID_SOCKET;
PlayerSession playerSessionArr[30] = { 0, };
int playerSessionCnt = 0;
int g_ID = 0;
fd_set readSet;

void Network();
void cs_Initial();
void Render();
void AcceptProc();
void SendUnicast(PlayerSession* playerSession, MSG_BASE* msg, int* msgSize);
void SendBroadcast(PlayerSession* playerSession, MSG_BASE* msg, int* msgSize);
void ReadProc(PlayerSession* playerSession);

int main() {
    setlocale(LC_ALL, "KOREAN");

    // 윈속 초기화
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        wprintf_s(L"WSAStartup() error:%d\n", WSAGetLastError());
        return 1;
    }

    // 서버 소켓 생성
    servSock = socket(AF_INET, SOCK_STREAM, 0);
    if (servSock == INVALID_SOCKET) {
        wprintf_s(L"socket() error:%d\n", WSAGetLastError());
        return 1;
    }

    // 넌블로킹 소켓 설정
    u_long mode = 1;
    ioctlsocket(servSock, FIONBIO, &mode);

    // 서버 주소 설정
    SOCKADDR_IN servAdr;
    ZeroMemory(&servAdr, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAdr.sin_port = htons(_wtoi(PORT));

    // 바인드
    if (bind(servSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR) {
        wprintf_s(L"bind() error:%d\n", WSAGetLastError());
        return 1;
    }

    // 리슨
    if (listen(servSock, 5) == SOCKET_ERROR) {
        wprintf_s(L"listen() error:%d\n", WSAGetLastError());
        return 1;
    }

    // 커서 숨기기
    cs_Initial();

    while (1) {
        Network();
        Render();
    }

    return 0;
}

void Network() {
    fd_set cpySet;
    FD_ZERO(&readSet);
    TIMEVAL timeout = { 0, 0 };

    FD_SET(servSock, &readSet);

    for (int i = 0; i < playerSessionCnt; i++) {
        FD_SET(playerSessionArr[i].clntSock, &readSet);
    }

    cpySet = readSet;

    int retSelect = select(0, &cpySet, NULL, NULL, &timeout);
    if (retSelect == SOCKET_ERROR) {
        wprintf_s(L"select() error:%d\n", WSAGetLastError());
        return;
    }

    if (retSelect > 0) {
        for (int i = 0; i < readSet.fd_count; i++) {
            if (FD_ISSET(readSet.fd_array[i], &cpySet)) {
                if (readSet.fd_array[i] == servSock) {
                    AcceptProc();
                }
                else {
                    for (int j = 0; j < playerSessionCnt; j++) {
                        if (readSet.fd_array[i] == playerSessionArr[j].clntSock) {
                            ReadProc(&playerSessionArr[j]);
                        }
                    }
                }
            }
        }
    }
}

void AcceptProc() {
    SOCKET clntSock;
    SOCKADDR_IN clntAdr;
    int adrSize = sizeof(clntAdr);

    while (1) {
        clntSock = accept(servSock, (SOCKADDR*)&clntAdr, &adrSize);
        if (clntSock == INVALID_SOCKET) {
            if (WSAGetLastError() == WSAEWOULDBLOCK) break;
            wprintf_s(L"accept() error:%d\n", WSAGetLastError());
            continue;
        }

        playerSessionArr[playerSessionCnt].clntAdr = clntAdr;
        playerSessionArr[playerSessionCnt].clntSock = clntSock;
        playerSessionArr[playerSessionCnt].id = g_ID;
        playerSessionArr[playerSessionCnt].x = INIT_X;
        playerSessionArr[playerSessionCnt].y = INIT_Y;

        MSG_ALLOC_ID msgAllocID;
        int msgSize = sizeof(msgAllocID);
        msgAllocID.type = ALLOC_ID;
        msgAllocID.id = g_ID;

        SendUnicast(&playerSessionArr[playerSessionCnt], (MSG_BASE*)&msgAllocID, &msgSize);

        MSG_CREATE_STAR msgCreateStar;
        msgSize = sizeof(msgCreateStar);
        for (int i = 0; i < playerSessionCnt; i++) {
            msgCreateStar.type = CREATE_STAR;
            msgCreateStar.id = playerSessionArr[i].id;
            msgCreateStar.x = playerSessionArr[i].x;
            msgCreateStar.y = playerSessionArr[i].y;
            SendUnicast(&playerSessionArr[playerSessionCnt], (MSG_BASE*)&msgCreateStar, &msgSize);
        }

        playerSessionCnt++;

        msgCreateStar.type = CREATE_STAR;
        msgCreateStar.id = g_ID;
        msgCreateStar.x = INIT_X;
        msgCreateStar.y = INIT_Y;

        SendBroadcast(NULL, (MSG_BASE*)&msgCreateStar, &msgSize);

        g_ID++;
    }
}

void SendUnicast(PlayerSession* playerSession, MSG_BASE* msg, int* msgSize) {
    int retSend = send(playerSession->clntSock, (char*)msg, *msgSize, 0);
    if (retSend == SOCKET_ERROR) {
        wprintf_s(L"send() error:%d\n", WSAGetLastError());
    }
}

void SendBroadcast(PlayerSession* playerSession, MSG_BASE* msg, int* msgSize) {
    for (int i = 0; i < playerSessionCnt; i++) {
        if (playerSession == &playerSessionArr[i]) continue;
        int retSend = send(playerSessionArr[i].clntSock, (char*)msg, *msgSize, 0);
        if (retSend == SOCKET_ERROR) {
            wprintf_s(L"send() error:%d\n", WSAGetLastError());
        }
    }
}

void ReadProc(PlayerSession* playerSession) {
    char msg[16];
    while (1) {
        int retRecv = recv(playerSession->clntSock, msg, sizeof(msg), 0);

        if (WSAGetLastError() == WSAEWOULDBLOCK || retRecv == 0) break;

        if (retRecv == SOCKET_ERROR) {
            wprintf_s(L"recv():%d\n", WSAGetLastError());

            MSG_DELETE_STAR msgDeleteStar;
            int msgSize = sizeof(msgDeleteStar);
            msgDeleteStar.type = DELETE_STAR;  
            msgDeleteStar.id = playerSession->id;
            SendBroadcast(playerSession, (MSG_BASE*)&msgDeleteStar, &msgSize);

            closesocket(playerSession->clntSock);
            for (int i = 0; i < playerSessionCnt; i++) {
                if (playerSession->id == playerSessionArr[i].id) {
                    playerSessionArr[i] = playerSessionArr[playerSessionCnt - 1];
                    playerSessionCnt--;
                    break;
                }
            }

            FD_CLR(playerSession->clntSock, &readSet);
            break;
        }

        int type = *(int*)msg;

        switch (type) {
        case MOVE_STAR: {
            MSG_MOVE_STAR* recvMSG = (MSG_MOVE_STAR*)msg;
            for (int i = 0; i < playerSessionCnt; i++) {
                if (recvMSG->id == playerSessionArr[i].id) {
                    playerSessionArr[i].x = recvMSG->x;
                    playerSessionArr[i].y = recvMSG->y;
                }
            }

            MSG_MOVE_STAR msgMoveStar;
            int msgSize = sizeof(msgMoveStar);
            msgMoveStar.type = MOVE_STAR;
            msgMoveStar.id = playerSession->id;
            msgMoveStar.x = recvMSG->x;
            msgMoveStar.y = recvMSG->y;

            SendBroadcast(playerSession, (MSG_BASE*)&msgMoveStar, &msgSize);
            break;
        }
        default:
            break;
        }
    }
}

void Render() {
    system("cls");

    wchar_t printArr[HEIGHT][WIDTH];
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH - 1; x++) {
            printArr[y][x] = L' ';
        }
        printArr[y][WIDTH - 1] = L'\0';
    }

    for (int i = 0; i < playerSessionCnt; i++) {
        printArr[playerSessionArr[i].y][playerSessionArr[i].x] = L'*';
    }

    for (int y = 0; y < HEIGHT; y++) {
        wprintf_s(L"%s\n", printArr[y]);
    }
}

HANDLE hConsole;
void cs_Initial() {
    CONSOLE_CURSOR_INFO stConsoleCursor;
    stConsoleCursor.bVisible = FALSE;
    stConsoleCursor.dwSize = 1;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorInfo(hConsole, &stConsoleCursor);
}