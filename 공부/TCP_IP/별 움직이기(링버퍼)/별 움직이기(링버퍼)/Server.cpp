#pragma comment(lib, "ws2_32")
#include<stdio.h>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<locale>
#include "CRingBuffer.h"


#define PORT L"3000"

#define BUF_SIZE 5000//100-> 5명, 50-> 2명//한방에 전송되는 메시지가 많으면 이것도 늘려야함

#define WIDTH 81
#define HEIGHT 24

#define INIT_X 40
#define INIT_Y 12

enum MSG_TYPE
{
	ALLOC_ID, CREATE_STAR, DELETE_STAR, MOVE_STAR
};

struct CLIENT
{
	int x = 0;
	int y = 0;
	int id = 0;
};

struct MSG_BASE
{
	int zero[4] = { 0, };
};

struct MSG_ALLOC_ID
{
	int type = ALLOC_ID;
	int id = 0;
	int zero[2] = { 0, };
};

struct MSG_CREATE_STAR
{
	int type = CREATE_STAR;
	int id = 0;
	int x = 0;
	int y = 0;
};

struct MSG_DELETE_STAR
{
	int type = DELETE_STAR;
	int id = 0;
	int zero[2] = { 0, };
};

struct MSG_MOVE_STAR
{
	int type = MOVE_STAR;
	int id = 0;
	int x = 0;
	int y = 0;
};

struct PlayerSession
{
	//ip, port, socekt, id, x, y
	SOCKADDR_IN clntAdr;
	SOCKET clntSock = INVALID_SOCKET;
	int id = 0;
	int x = 0;
	int y = 0;
	//링버퍼
	CRingBuffer recvQ;
	CRingBuffer sendQ;
};
//-------------------------------------------//
SOCKET servSock = INVALID_SOCKET;
PlayerSession playerSessionArr[30] = { 0, };
int playerSessionCnt = 0;
int g_ID = 0;
fd_set readSet;

void Network();
//-------------------------------------------//
wchar_t printArr[HEIGHT][WIDTH];
void cs_Initial();
void Render();
//-------------------------------------------//
void AcceptProc();
void SendUnicast(PlayerSession* playerSession, MSG_BASE* msg, int* msgSize);
void SendBroadcast(PlayerSession* playerSession, MSG_BASE* msg, int* msgSize);
void ReadProc(PlayerSession* playerSession);

int main()
{
	setlocale(LC_ALL, "KOREAN");
	//윈속 초기화
	WSADATA wsaData;
	int retWSAStartup = 0;
	retWSAStartup = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (retWSAStartup != 0)
		wprintf_s(L"WSAStartup() error:%d\n", WSAGetLastError());

	//서버소켓만들기
	servSock = INVALID_SOCKET;
	servSock = socket(AF_INET, SOCK_STREAM, 0);
	if (servSock == SOCKET_ERROR)
		wprintf_s(L"socket() error:%d\n", WSAGetLastError());

	//넌블로킹소켓설정
	u_long mode = 1;
	ioctlsocket(servSock, FIONBIO, &mode);

	//서버 주소 설정
	SOCKADDR_IN servAdr;
	ZeroMemory(&servAdr, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(_wtoi(PORT));

	//바인드
	int retBind = 0;
	retBind = bind(servSock, (SOCKADDR*)&servAdr, sizeof(servAdr));
	if (retBind == SOCKET_ERROR)
		wprintf_s(L"bind() error:%d\n", WSAGetLastError());

	//소켓 리슨
	int retListen = 0;
	retListen = listen(servSock, 5);
	if (retListen == SOCKET_ERROR)
		wprintf_s(L"listen() error:%d\n", WSAGetLastError());

	//커서를 안보이게
	cs_Initial();

	while (1)
	{
		Network();
		Render();
	}
}

void Network()
{
	readSet;
	fd_set cpySet;
	FD_ZERO(&readSet);
	TIMEVAL timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	//서버소켓을 read세트에 넣는다.
	//wprintf_s(L"서버소켓 넣음:%d\n", servSock);
	FD_SET(servSock, &readSet);

	//플레이어 세션들의 소켓을 read세트에 넣는다.
	for (int i = 0; i < playerSessionCnt; i++)
	{
		//wprintf_s(L"플레이어 세션 넣음:%d\n", playerSessionArr[i].clntSock);
		FD_SET(playerSessionArr[i].clntSock, &readSet);
	}

	//세트를 카피한다.
	cpySet = readSet;

	//데이터 또는 커넥트요청이 들어오지 않는다면 멈춰있겠다 -> 타임밸류 NULL
	int retSelect = 0;
	retSelect = select(0, &cpySet, NULL, NULL, &timeout);
	if (retSelect == SOCKET_ERROR)
		wprintf_s(L"select() error:%d\n", WSAGetLastError());

	//readSet에 뭔가 요청이 있다.
	if (retSelect > 0)
	{
		//readSet 탐색
		for (int i = 0; i < readSet.fd_count; i++)
		{
			//반응이 있는 소켓을 세트내에서 찾는다.
			if (FD_ISSET(readSet.fd_array[i], &cpySet))
			{
				//확인해보니 서버소켓이었다면(클라이언트의 connect 요청이었다면)
				if (readSet.fd_array[i] == servSock)
				{
					//wprintf_s(L"클라이언트였다.:%d\n", retSelect);
					//accept 활동<<<<<<<<<<<<<<여기 왜 안들어가는데!!!(최적화 끄기로 해결)
					AcceptProc();
				}

				//플레이어 세션 배열에 있는 모든 세션에 대해서 검사
				for (int j = 0; j < playerSessionCnt; j++)
				{
					//확인해보니 플레이어 세션이었다면
					if (readSet.fd_array[i] == playerSessionArr[j].clntSock)
					{
						//wprintf_s(L"세션이었다.:%d\n", retSelect);
						//read 활동
						ReadProc(&playerSessionArr[j]);
					}
				}

			}
		}
	}
	//-----------------------------------------------------------------------------//
	fd_set writeSet;
	FD_ZERO(&writeSet);

	//플레이어 세션들의 sendQ 사용여부를 확인하고 세트에 넣는다.
	for (int i = 0; i < playerSessionCnt; i++)
	{
		if (playerSessionArr[i].sendQ.GetUseSize() > 0)
			FD_SET(playerSessionArr[i].clntSock, &writeSet);
	}

	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	cpySet = writeSet;

	retSelect = select(0, NULL, &cpySet, NULL, &timeout);
	if (retSelect > 0)
	{
		char buffer[BUF_SIZE];
		for (int i = 0; i < writeSet.fd_count; i++)
		{
			if (FD_ISSET(writeSet.fd_array[i], &cpySet))
			{
				for (int j = 0; j < playerSessionCnt; j++)
				{
					if (playerSessionArr[j].clntSock != writeSet.fd_array[j])
						continue;

					int retPeek = 0;
					retPeek = playerSessionArr[j].sendQ.Peek(buffer, playerSessionArr[j].sendQ.GetUseSize());

					int retSend = 0;
					retSend = send(playerSessionArr[j].clntSock, buffer, retPeek, 0);
					if (retSend == SOCKET_ERROR)
						wprintf_s(L"send() error:%d\n", WSAGetLastError());

					playerSessionArr[j].sendQ.MoveFront(retSend);
				}
			}
		}
	}

	//-----------------------------------------------------------------------------//
}

void AcceptProc()
{
	//클라이언트 소켓, 주소, 사이즈 임시 저장
	SOCKET clntSock = INVALID_SOCKET;
	SOCKADDR_IN clntAdr;
	int adrSize = sizeof(clntAdr);

	while (1)
	{
		clntSock = accept(servSock, (SOCKADDR*)&clntAdr, &adrSize);

		if (WSAGetLastError() == WSAEWOULDBLOCK)
			break;

		if (clntSock == INVALID_SOCKET)
			wprintf_s(L"accept() error:%d\n", WSAGetLastError());

		//accept 후 세션배열에 주소 정보, 소켓, ID, 좌표 등록
		playerSessionArr[playerSessionCnt].clntAdr = clntAdr;
		playerSessionArr[playerSessionCnt].clntSock = clntSock;
		playerSessionArr[playerSessionCnt].id = g_ID;
		playerSessionArr[playerSessionCnt].x = INIT_X;
		playerSessionArr[playerSessionCnt].y = INIT_Y;
		//링버퍼 초기화
		playerSessionArr[playerSessionCnt].recvQ = CRingBuffer(5000);
		playerSessionArr[playerSessionCnt].sendQ = CRingBuffer(5000);


		//ID할당 메시지 작성
		MSG_ALLOC_ID msgAllocID;
		int msgSize = sizeof(msgAllocID);
		msgAllocID.type = ALLOC_ID;
		msgAllocID.id = g_ID;

		//send 유니캐스트(한명에게만)
		/*wprintf_s(L"메시지 주소:%p\n", &msgAllocID);
		wprintf_s(L"메시지 사이즈:%d\n", msgSize);*/
		SendUnicast(&playerSessionArr[playerSessionCnt], (MSG_BASE*)&msgAllocID, &msgSize);

		//별 생성 메시지
		MSG_CREATE_STAR msgCreateStar;
		msgSize = sizeof(msgCreateStar);
		//원래 존재하던 녀석들의 정보도 신입에게 알려야한다.
		//왜 움직여야 원래 존재하던 녀석들이 보이는가??생성하고 MOVE한번 해줘야하나??왼쪽위에 있었다.
		for (int i = 0; i < playerSessionCnt; i++)
		{
			//메시지 작성
			msgCreateStar.type = CREATE_STAR;
			msgCreateStar.id = playerSessionArr[i].id;
			msgCreateStar.x = playerSessionArr[i].x;
			msgCreateStar.y = playerSessionArr[i].y;
			wprintf_s(L"playerCnt:%d\n", playerSessionCnt);
			wprintf_s(L"id:%d, x:%d, y:%d\n", msgCreateStar.id, msgCreateStar.x, msgCreateStar.y);
			SendUnicast(&playerSessionArr[playerSessionCnt], (MSG_BASE*)&msgCreateStar, &msgSize);
		}

		playerSessionCnt++;//<<<<<<<<<<<이 녀석의 위치 중요하다!! 브로드 캐스트가 안먹었다.

		//신입의 생성을 모두에게 알려야한다.
		//별 생성 메시지 작성
		msgCreateStar.type = CREATE_STAR;
		msgCreateStar.id = g_ID;
		msgCreateStar.x = INIT_X;
		msgCreateStar.y = INIT_Y;

		//wprintf_s(L"메시지 주소:%p\n", &msgCreateStar);
		//wprintf_s(L"메시지 사이즈:%d\n", msgSize);
		//첫 인자의 한명을 제외하고 모두보냄(NULL이면 모두보냄)
		SendBroadcast(NULL, (MSG_BASE*)&msgCreateStar, &msgSize);

		g_ID++;
	}
}

void SendUnicast(PlayerSession* playerSession, MSG_BASE* msg, int* msgSize)
{
	//-------------------------------------------//
	int retEnQ = 0;
	retEnQ = playerSession->sendQ.Enqueue((char*)msg, *msgSize);
	if (retEnQ < *msgSize)
	{
		wprintf_s(L"sendQ.Enqueue() error\n");
		exit(1);
	}
	//-------------------------------------------//

	////wprintf_s(L"메시지주소:%p\n", msg);
	////wprintf_s(L"메시지 사이즈:%d\n", *msgSize);
	//int retSend = 0;
	////wprintf_s(L"유니 받는사람:%d\n", playerSession->clntSock);
	//retSend = send(playerSession->clntSock, (char*)msg, *msgSize, 0);
	//if (retSend == SOCKET_ERROR)
	//	wprintf_s(L"send() error:%d\n", WSAGetLastError());
	////wprintf_s(L"전송결과:%d\n", retSend);
}

void SendBroadcast(PlayerSession* playerSession, MSG_BASE* msg, int* msgSize)
{
	//-------------------------------------------//
	int retEnQ = 0;
	for (int i = 0; i < playerSessionCnt; i++)
	{
		if (playerSession == &playerSessionArr[i])
			continue;
		retEnQ = playerSessionArr[i].sendQ.Enqueue((char*)msg, *msgSize);
		if (retEnQ < *msgSize)
		{
			wprintf_s(L"sendQ.Enqueue() error\n");
			exit(1);
		}
	}
	//-------------------------------------------//

	//wprintf_s(L"메시지주소:%p\n", msg);
	//wprintf_s(L"메시지 사이즈:%d\n", *msgSize);
	//int retSend = 0;
	//for (int i = 0; i < playerSessionCnt; i++)
	//{
	//	//wprintf_s(L"i:%d\n", i);
	//	//주소값 비교
	//	//wprintf_s(L"주소값 비교:%p\n", playerSession);
	//	//wprintf_s(L"주소값 비교:%p\n", &playerSessionArr[i]);
	//	if (playerSession == &playerSessionArr[i])
	//		continue;
	//	//wprintf_s(L"브로드 받는사람:%d\n", playerSessionArr[i].clntSock);
	//	retSend = send(playerSessionArr[i].clntSock, (char*)msg, *msgSize, 0);
	//	if (retSend == SOCKET_ERROR)
	//		wprintf_s(L"send() error:%d\n", WSAGetLastError());
	//	//wprintf_s(L"전송결과:%d\n", retSend);
	//}
}

void ReadProc(PlayerSession* playerSession)
{
	char buffer[BUF_SIZE];//msg[max]??뭐지??
	while (1)
	{
		int retRecv = 0;
		retRecv = recv(playerSession->clntSock, buffer, sizeof(buffer), 0);

		if (WSAGetLastError() == WSAEWOULDBLOCK || retRecv == 0 /*|| retRecv == -1*/)
			break;

		if (retRecv == SOCKET_ERROR)
		{
			wprintf_s(L"recv():%d\n", WSAGetLastError());

			//데이터 받기 에러나면 모두에게 브로드캐스트로 이 별 삭제하라고 전달
			//메시지 작성
			MSG_DELETE_STAR msgDeleteStar;
			int msgSize = sizeof(msgDeleteStar);
			msgDeleteStar.type = DELETE_STAR;
			msgDeleteStar.id = playerSession->id;
			//메시지 전송
			SendBroadcast(playerSession, (MSG_BASE*)&msgDeleteStar, &msgSize);
			//소켓 연결 끊기
			closesocket(playerSession->clntSock);
			//세션 배열에서 제거
			for (int i = 0; i < playerSessionCnt; i++)
			{
				if (playerSession->id == playerSessionArr[i].id)
				{
					playerSessionArr[i] = playerSession[playerSessionCnt - 1];
					playerSessionCnt--;
				}
			}
			//read세트에서 제거
			//wprintf_s(L"세트에서 제거:%d\n", playerSession->clntSock);
			FD_CLR(playerSession->clntSock, &readSet);

			break;
		}
		//-----------------------------------------------------------------------------//
		playerSession->recvQ.Enqueue(buffer, retRecv);
		while (1)
		{
			if (playerSession->recvQ.GetUseSize() < 16)
				break;//중단
			char msg[16];
			int retDeQ = 0;
			retDeQ = playerSession->recvQ.Dequeue(msg, 16);
			if (retDeQ != 16)
				exit(1);//결함
			//-----------------------------------------------------------------------------//

			int type = *(int*)msg;
			//wprintf(L"받은 메시지 타입: %d\n", type);

			switch (type)
			{
			case MOVE_STAR://클라가 별을 움직였다
			{
				//받은 메시지 해석
				MSG_MOVE_STAR* recvMSG = (MSG_MOVE_STAR*)msg;
				//세션 배열에 좌표 정보 저장
				for (int i = 0; i < playerSessionCnt; i++)
				{
					if (recvMSG->id == playerSessionArr[i].id)
					{
						playerSessionArr[i].x = recvMSG->x;
						playerSessionArr[i].y = recvMSG->y;
					}
				}

				//메시지 작성
				MSG_MOVE_STAR msgMoveStar;
				int msgSize = sizeof(msgMoveStar);
				msgMoveStar.type = MOVE_STAR;
				msgMoveStar.id = playerSession->id;
				msgMoveStar.x = recvMSG->x;
				msgMoveStar.y = recvMSG->y;

				//그 클라를 제외하고 브로드캐스팅
				SendBroadcast(playerSession, (MSG_BASE*)&msgMoveStar, &msgSize);
				break;
			}
			default:
				break;
			}
		}
	}
}

void Render()
{
	//화면 지우기
	system("cls");

	//배열 ' '로 초기화, 마지막은 '\0'
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH - 1; x++)
		{
			printArr[y][x] = L' ';
		}
		printArr[y][WIDTH - 1] = L'\0';
	}

	//플레이어들 위치에 * 넣기
	for (int i = 0; i < playerSessionCnt; i++)
	{
		printArr[playerSessionArr[i].y][playerSessionArr[i].x] = L'*';
	}

	//화면에 출력
	for (int y = 0; y < HEIGHT; y++)
	{
		wprintf_s(L"%s\n", printArr[y]);
	}
}

HANDLE hConsole;
void cs_Initial()
{
	CONSOLE_CURSOR_INFO stConsoleCursor;
	//화면의 커서를 안보이게 설정
	stConsoleCursor.bVisible = FALSE;
	stConsoleCursor.dwSize = 1;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(hConsole, &stConsoleCursor);
}