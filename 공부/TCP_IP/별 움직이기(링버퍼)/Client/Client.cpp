#pragma comment(lib, "ws2_32")
#include<stdio.h>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<locale>
#include"CRingBuffer.h"

#define IP L"127.0.0.1"
#define PORT L"3000"

#define WIDTH 80
#define HEIGHT 23

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

//------------------------//
SOCKET sock;
void Network();
//------------------------//
void Input();
//------------------------//
void Render();
//------------------------//
int myID;
CLIENT clientArr[30];
int clientCnt = 0;
wchar_t printArr[HEIGHT][WIDTH];
//------------------------//
void cs_Initial();
//------------------------//
CRingBuffer sendQ;
CRingBuffer recvQ;

#define BUF_SIZE 100 // 100->11명(enque 사이즈 초과)

void printArrSet()
{
	//출력 배열 초기화
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			printArr[i][j] = (wchar_t)L' ';
			if (j == WIDTH - 1)
				printArr[i][j] = (wchar_t)L'\0';
		}
	}
}

int main()
{
	//링버퍼 초기화
	sendQ = CRingBuffer();
	recvQ = CRingBuffer(1000);//100->11명까지 1000->110?? 26명까진 됬음
	//커서제거
	cs_Initial();
	//출력 배열 초기화
	printArrSet();

	setlocale(LC_ALL, "KOREAN");
	//윈속 초기화
	WSADATA wsaData;
	int retStartup = 0;
	retStartup = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (retStartup != 0)
		wprintf_s(L"WSAStartup() error:%d\n", WSAGetLastError());
	//소켓 생성
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == SOCKET_ERROR)
		wprintf_s(L"socket() error:%d\n", WSAGetLastError());

	//논블로킹으로 전환<<<<<<<<이것으로 다른 클라 반응 느리던거 해결
	u_long mode = 1;
	ioctlsocket(sock, FIONBIO, &mode);

	//주소입력
	SOCKADDR_IN adr;
	ZeroMemory(&adr, sizeof(adr));
	adr.sin_family = AF_INET;
	InetPton(AF_INET, (PCWSTR)IP, &adr.sin_addr.s_addr);
	adr.sin_port = htons(_wtoi(PORT));
	//서버에 연결
	int retConnect = 0;
	retConnect = connect(sock, (SOCKADDR*)&adr, sizeof(adr));
	if (retConnect == SOCKET_ERROR)
		wprintf_s(L"connect() error:%d\n", WSAGetLastError());



	//클라이언트 배열 초기화
	ZeroMemory(clientArr, sizeof(clientArr));

	//연결이 되고 난 후
	while (1)
	{
		Input();
		Network();
		Render();
		//Sleep(500);
		system("cls");
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

void Render()
{
	wprintf_s(L"클라이언트 수: %d\n", clientCnt);

	printArrSet();

	for (int h = 0; h < clientCnt; h++)
	{
		printArr[clientArr[h].y][clientArr[h].x] = (wchar_t)L'*';
	}

	for (int i = 0; i < HEIGHT; i++)
	{
		wprintf_s(L"%s", printArr[i]);
		_putws(L"");
	}
}

void Input()
{
	CLIENT* myClient = NULL;
	for (int i = 0; i < clientCnt; i++)
	{
		if (clientArr[i].id == myID)
			myClient = &clientArr[i];
	}

	if (myClient == NULL)
		return;

	CLIENT tempClient = *myClient;

	if (GetAsyncKeyState(VK_LEFT))
		tempClient.x--;
	if (GetAsyncKeyState(VK_RIGHT))
		tempClient.x++;
	if (GetAsyncKeyState(VK_UP))
		tempClient.y--;
	if (GetAsyncKeyState(VK_DOWN))
		tempClient.y++;

	//좌표값 제한
	if (tempClient.x > WIDTH - 1)
		tempClient.x = WIDTH - 1;
	if (tempClient.x < 0)
		tempClient.x = 0;

	if (tempClient.y > HEIGHT - 1)
		tempClient.y = HEIGHT - 1;
	if (tempClient.y < 0)
		tempClient.y = 0;

	//wprintf_s(L"id:%d x:%d, y:%d\n", myClient->id, myClient->x, myClient->y);

	//임시 클라이언트에 저장
	int retSend = 0;
	MSG_MOVE_STAR msgMoveStar;
	msgMoveStar.id = tempClient.id;
	msgMoveStar.x = tempClient.x;
	msgMoveStar.y = tempClient.y;

	//좌표값이 바뀌었을 때 전송
	if (myClient->x != tempClient.x || myClient->y != tempClient.y)
	{
		//-------------------------------------------------//
		int retEnQ = 0;
		retEnQ = sendQ.Enqueue((char*)&msgMoveStar, sizeof(msgMoveStar));
		if (retEnQ < sizeof(msgMoveStar))
			wprintf_s(L"sendQ.Enqueue() error\n");
		//-------------------------------------------------//
		/*retSend = send(sock, (char*)&msgMoveStar, sizeof(msgMoveStar), 0);
		if (retSend == SOCKET_ERROR)
			wprintf_s(L"send() error:%d", WSAGetLastError());*/

			//_putws(L"send");
	}
	//바뀐값 클라이언트에 적용
	*myClient = tempClient;
}

void Network()
{
	fd_set readSet;
	fd_set tempSet;
	timeval timeout;

	FD_ZERO(&readSet);
	FD_SET(sock, &readSet);

	char buffer[BUF_SIZE] = { 0, };
	while (1)
	{
		tempSet = readSet;
		timeout.tv_sec = 0;
		timeout.tv_usec = 0;

		int retSelect = 0;
		retSelect = select(0, &tempSet, 0, 0, &timeout);

		if (WSAGetLastError() == WSAEWOULDBLOCK || retSelect == 0)//<<<<<<<<이것으로 다른 클라 반응 느리던거 해결
			break;

		if (retSelect == SOCKET_ERROR)
			wprintf_s(L"select() error:%d\n", WSAGetLastError());

		/*if (retSelect == 0)
			wprintf_s(L"timeout!\n");*/

		if (FD_ISSET(sock, &tempSet))
		{
			int retRecv = 0;
			retRecv = recv(sock, buffer, sizeof(buffer), 0);
			if (retRecv == SOCKET_ERROR)
				wprintf_s(L"recv() error:%d\n", WSAGetLastError());
			//-----------------------------------------------------//
			recvQ.Enqueue(buffer, retRecv);

			while (1)
			{
				if (recvQ.GetUseSize() < 16)
					break;//중단
				char msg[16];
				int retDeQ = 0;
				retDeQ = recvQ.Dequeue(msg, 16);
				if (retDeQ < 16)
					exit(1);//결함
				//-----------------------------------------------------//
				int type = *(int*)msg;
				//wprintf(L"받은 메시지 타입: %d\n", type);

				switch (type)
				{
				case ALLOC_ID:
				{
					//wprintf(L"타입이 할당이네\n");
					MSG_ALLOC_ID* msgAllocID = (MSG_ALLOC_ID*)msg;
					//내 아이디 저장
					myID = msgAllocID->id;
					//wprintf_s(L"아이디는 %d\n", msgAllocID->id);
					break;
				}
				case CREATE_STAR:
				{
					//wprintf(L"타입이 별 생성 이네\n");
					MSG_CREATE_STAR* msgCreateStar = (MSG_CREATE_STAR*)msg;
					clientArr[clientCnt].id = msgCreateStar->id;
					clientArr[clientCnt].x = msgCreateStar->x;
					clientArr[clientCnt].y = msgCreateStar->y;

					//wprintf(L"id:%d, x:%d, y:%d\n", msgCreateStar->id, msgCreateStar->x, msgCreateStar->y);
					clientCnt++;
					break;
				}
				case MOVE_STAR:
				{
					MSG_MOVE_STAR* msgMoveStar = (MSG_MOVE_STAR*)msg;
					for (int k = 0; k < clientCnt; k++)
					{
						if (msgMoveStar->id == clientArr[k].id)
						{
							clientArr[k].x = msgMoveStar->x;
							clientArr[k].y = msgMoveStar->y;
							break;
						}
					}
					break;
				}
				case DELETE_STAR:
				{
					MSG_DELETE_STAR* msgDeleteStar = (MSG_DELETE_STAR*)msg;
					for (int i = 0; i < clientCnt; i++)
					{
						if (msgDeleteStar->id == clientArr[i].id)
						{
							clientArr[i] = clientArr[clientCnt - 1];
							clientCnt--;
							break;
						}
					}
					break;
				}
				default:
					wprintf(L"msg type: %d\n", type);
					closesocket(sock);
					WSACleanup();
					break;
				}
			}
		}
	}

	//-------------------------------------------------//
	fd_set writeSet;
	FD_ZERO(&writeSet);
	fd_set cpySet;
	FD_SET(sock, &writeSet);

	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	cpySet = writeSet;
	int retSelect = 0;
	retSelect = select(0, NULL, &cpySet, NULL, &timeout);
	if (retSelect == SOCKET_ERROR)
	{
		wprintf_s(L"select error():%d\n", WSAGetLastError());
		exit(1);
	}

	if (retSelect == 0)
		return;

	char sendBuffer[BUF_SIZE];
	int retPeek = 0;
	retPeek = sendQ.Peek(sendBuffer, sendQ.GetUseSize());

	int retSend = 0;
	retSend = send(sock, sendBuffer, retPeek, 0);
	if (retSend == SOCKET_ERROR)
	{
		wprintf_s(L"send() error:%d\n", WSAGetLastError());
		exit(1);
	}

	sendQ.MoveFront(retPeek);

	//-------------------------------------------------//
}