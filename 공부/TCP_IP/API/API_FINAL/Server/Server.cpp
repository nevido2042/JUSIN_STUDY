#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <locale>
#include "MSG.h"

using namespace std;

#define PORT L"2042"

struct SESSION
{
    SOCKADDR_IN clntAdr;
    SOCKET clntSock = INVALID_SOCKET;
    int id = 0;
    int x = 0;
    int y = 0;
};

SOCKET servSock = INVALID_SOCKET;
SESSION playerSessionArr[30] = { 0, };
int playerSessionCnt = 0;
int g_ID = 0;
fd_set readSet;
  
void Network();
void AcceptProc();
void SendUnicast(const SESSION* playerSession, MSG_BASE* msg, int* msgSize);
void SendBroadcast(const SESSION* playerSession, MSG_BASE* msg, int* msgSize);
void ReadProc(SESSION* playerSession);

int main() 
{
    cout << "서버" << endl;
    setlocale(LC_ALL, "KOREAN");

    // 윈속 초기화
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
    {
        wprintf_s(L"WSAStartup() error:%d\n", WSAGetLastError());
        return 1;
    }

    // 서버 소켓 생성
    servSock = socket(AF_INET, SOCK_STREAM, 0);
    if (servSock == INVALID_SOCKET) 
    {
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
    if (bind(servSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
    {
        wprintf_s(L"bind() error:%d\n", WSAGetLastError());
        return 1;
    }

    // 리슨
    if (listen(servSock, 5) == SOCKET_ERROR) 
    {
        wprintf_s(L"listen() error:%d\n", WSAGetLastError());
        return 1;
    }

    while (true) 
    {
        Network();
    }

    return 0;
}

void Network()
{
    fd_set cpySet;
    FD_ZERO(&readSet);
    TIMEVAL timeout = { 0, 0 };

    FD_SET(servSock, &readSet);

    for (int i = 0; i < playerSessionCnt; i++) 
    {
        FD_SET(playerSessionArr[i].clntSock, &readSet);
    }

    cpySet = readSet;

    int retSelect = select(0, &cpySet, NULL, NULL, &timeout);
    if (retSelect == SOCKET_ERROR) {
        wprintf_s(L"select() error:%d\n", WSAGetLastError());
        return;
    }

    if (retSelect > 0) 
    {
        for (int i = 0; i < readSet.fd_count; i++)
        {
            if (FD_ISSET(readSet.fd_array[i], &cpySet))
            {
                if (readSet.fd_array[i] == servSock)
                {
                    AcceptProc();
                }
                else 
                {
                    for (int j = 0; j < playerSessionCnt; j++)
                    {
                        if (readSet.fd_array[i] == playerSessionArr[j].clntSock) 
                        {
                            ReadProc(&playerSessionArr[j]);
                        }
                    }
                }
            }
        }
    }
}

void AcceptProc() 
{
    SOCKET clntSock;
    SOCKADDR_IN clntAdr;
    int adrSize = sizeof(clntAdr);

    while (1) 
    {
        clntSock = accept(servSock, (SOCKADDR*)&clntAdr, &adrSize);
        if (clntSock == INVALID_SOCKET) {
            if (WSAGetLastError() == WSAEWOULDBLOCK) break;
            wprintf_s(L"accept() error:%d\n", WSAGetLastError());
            continue;
        }

        cout << "클라이언트 접속" << endl;

        //신입 정보
        playerSessionArr[playerSessionCnt].clntAdr = clntAdr;
        playerSessionArr[playerSessionCnt].clntSock = clntSock;
        playerSessionArr[playerSessionCnt].id = g_ID;
        playerSessionArr[playerSessionCnt].x = rand() % 800;
        playerSessionArr[playerSessionCnt].y = rand() % 600;
        //신입 정보 메시지
        MSG_ALLOC_ID msgAllocID;
        int msgSize = sizeof(msgAllocID);
        msgAllocID.type = ALLOC_ID;
        msgAllocID.id = g_ID;
        //신입한테 아이디 알려줌
        SendUnicast(&playerSessionArr[playerSessionCnt], (MSG_BASE*)&msgAllocID, &msgSize);
        //신입한테 모든 유저 출력하라고 함
        MSG_CREATE_STAR msgCreateStar;
        msgSize = sizeof(msgCreateStar);

        for (int i = 0; i < playerSessionCnt + 1; i++)
        {
            msgCreateStar.type = CREATE_STAR;
            msgCreateStar.id = playerSessionArr[i].id;
            msgCreateStar.x = playerSessionArr[i].x;
            msgCreateStar.y = playerSessionArr[i].y;
            SendUnicast(&playerSessionArr[playerSessionCnt], (MSG_BASE*)&msgCreateStar, &msgSize);
        }

        msgCreateStar.type = CREATE_STAR;
        msgCreateStar.id = g_ID;
        //msgCreateStar.x = playerSessionArr[playerSessionCnt].x;
        //msgCreateStar.y = playerSessionArr[playerSessionCnt].y;

        SendBroadcast(NULL, (MSG_BASE*)&msgCreateStar, &msgSize);

        playerSessionCnt++;
        g_ID++;
    }
}

void SendUnicast(SESSION* playerSession, MSG_BASE* msg, int* msgSize)
{
    int retSend = send(playerSession->clntSock, (char*)msg, *msgSize, 0);
    if (retSend == SOCKET_ERROR) 
    {
        wprintf_s(L"send() error:%d\n", WSAGetLastError());
    }
}

void SendBroadcast(SESSION* playerSession, MSG_BASE* msg, int* msgSize)
{
    for (int i = 0; i < playerSessionCnt; i++)
    {
        if (playerSession == &playerSessionArr[i]) continue;
        int retSend = send(playerSessionArr[i].clntSock, (char*)msg, *msgSize, 0);
        if (retSend == SOCKET_ERROR)
        {
            wprintf_s(L"send() error:%d\n", WSAGetLastError());
        }
    }
}


void ReadProc(SESSION* playerSession) 
{
    char msg[16];
    while (1) 
    {
        int retRecv = recv(playerSession->clntSock, msg, sizeof(msg), 0);

        if (WSAGetLastError() == WSAEWOULDBLOCK || retRecv == 0) break;

        if (retRecv == SOCKET_ERROR) 
        {
            wprintf_s(L"recv():%d\n", WSAGetLastError());

            closesocket(playerSession->clntSock);
            for (int i = 0; i < playerSessionCnt; i++)
            {
                if (playerSession->id == playerSessionArr[i].id) 
                {
                    playerSessionArr[i] = playerSessionArr[playerSessionCnt - 1];
                    playerSessionCnt--;
                    break;
                }
            }

            FD_CLR(playerSession->clntSock, &readSet);
            break;
        }

        int type = *(int*)msg;
    }
}
