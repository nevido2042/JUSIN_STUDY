#pragma comment(lib, "ws2_32")
#include<stdio.h>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<locale>
#include "CRingBuffer.h"


#define PORT L"3000"

#define BUF_SIZE 5000//100-> 5��, 50-> 2��//�ѹ濡 ���۵Ǵ� �޽����� ������ �̰͵� �÷�����

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
	//������
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
	//���� �ʱ�ȭ
	WSADATA wsaData;
	int retWSAStartup = 0;
	retWSAStartup = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (retWSAStartup != 0)
		wprintf_s(L"WSAStartup() error:%d\n", WSAGetLastError());

	//�������ϸ����
	servSock = INVALID_SOCKET;
	servSock = socket(AF_INET, SOCK_STREAM, 0);
	if (servSock == SOCKET_ERROR)
		wprintf_s(L"socket() error:%d\n", WSAGetLastError());

	//�ͺ��ŷ���ϼ���
	u_long mode = 1;
	ioctlsocket(servSock, FIONBIO, &mode);

	//���� �ּ� ����
	SOCKADDR_IN servAdr;
	ZeroMemory(&servAdr, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(_wtoi(PORT));

	//���ε�
	int retBind = 0;
	retBind = bind(servSock, (SOCKADDR*)&servAdr, sizeof(servAdr));
	if (retBind == SOCKET_ERROR)
		wprintf_s(L"bind() error:%d\n", WSAGetLastError());

	//���� ����
	int retListen = 0;
	retListen = listen(servSock, 5);
	if (retListen == SOCKET_ERROR)
		wprintf_s(L"listen() error:%d\n", WSAGetLastError());

	//Ŀ���� �Ⱥ��̰�
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
	//���������� read��Ʈ�� �ִ´�.
	//wprintf_s(L"�������� ����:%d\n", servSock);
	FD_SET(servSock, &readSet);

	//�÷��̾� ���ǵ��� ������ read��Ʈ�� �ִ´�.
	for (int i = 0; i < playerSessionCnt; i++)
	{
		//wprintf_s(L"�÷��̾� ���� ����:%d\n", playerSessionArr[i].clntSock);
		FD_SET(playerSessionArr[i].clntSock, &readSet);
	}

	//��Ʈ�� ī���Ѵ�.
	cpySet = readSet;

	//������ �Ǵ� Ŀ��Ʈ��û�� ������ �ʴ´ٸ� �����ְڴ� -> Ÿ�ӹ�� NULL
	int retSelect = 0;
	retSelect = select(0, &cpySet, NULL, NULL, &timeout);
	if (retSelect == SOCKET_ERROR)
		wprintf_s(L"select() error:%d\n", WSAGetLastError());

	//readSet�� ���� ��û�� �ִ�.
	if (retSelect > 0)
	{
		//readSet Ž��
		for (int i = 0; i < readSet.fd_count; i++)
		{
			//������ �ִ� ������ ��Ʈ������ ã�´�.
			if (FD_ISSET(readSet.fd_array[i], &cpySet))
			{
				//Ȯ���غ��� ���������̾��ٸ�(Ŭ���̾�Ʈ�� connect ��û�̾��ٸ�)
				if (readSet.fd_array[i] == servSock)
				{
					//wprintf_s(L"Ŭ���̾�Ʈ����.:%d\n", retSelect);
					//accept Ȱ��<<<<<<<<<<<<<<���� �� �ȵ��µ�!!!(����ȭ ����� �ذ�)
					AcceptProc();
				}

				//�÷��̾� ���� �迭�� �ִ� ��� ���ǿ� ���ؼ� �˻�
				for (int j = 0; j < playerSessionCnt; j++)
				{
					//Ȯ���غ��� �÷��̾� �����̾��ٸ�
					if (readSet.fd_array[i] == playerSessionArr[j].clntSock)
					{
						//wprintf_s(L"�����̾���.:%d\n", retSelect);
						//read Ȱ��
						ReadProc(&playerSessionArr[j]);
					}
				}

			}
		}
	}
	//-----------------------------------------------------------------------------//
	fd_set writeSet;
	FD_ZERO(&writeSet);

	//�÷��̾� ���ǵ��� sendQ ��뿩�θ� Ȯ���ϰ� ��Ʈ�� �ִ´�.
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
	//Ŭ���̾�Ʈ ����, �ּ�, ������ �ӽ� ����
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

		//accept �� ���ǹ迭�� �ּ� ����, ����, ID, ��ǥ ���
		playerSessionArr[playerSessionCnt].clntAdr = clntAdr;
		playerSessionArr[playerSessionCnt].clntSock = clntSock;
		playerSessionArr[playerSessionCnt].id = g_ID;
		playerSessionArr[playerSessionCnt].x = INIT_X;
		playerSessionArr[playerSessionCnt].y = INIT_Y;
		//������ �ʱ�ȭ
		playerSessionArr[playerSessionCnt].recvQ = CRingBuffer(5000);
		playerSessionArr[playerSessionCnt].sendQ = CRingBuffer(5000);


		//ID�Ҵ� �޽��� �ۼ�
		MSG_ALLOC_ID msgAllocID;
		int msgSize = sizeof(msgAllocID);
		msgAllocID.type = ALLOC_ID;
		msgAllocID.id = g_ID;

		//send ����ĳ��Ʈ(�Ѹ��Ը�)
		/*wprintf_s(L"�޽��� �ּ�:%p\n", &msgAllocID);
		wprintf_s(L"�޽��� ������:%d\n", msgSize);*/
		SendUnicast(&playerSessionArr[playerSessionCnt], (MSG_BASE*)&msgAllocID, &msgSize);

		//�� ���� �޽���
		MSG_CREATE_STAR msgCreateStar;
		msgSize = sizeof(msgCreateStar);
		//���� �����ϴ� �༮���� ������ ���Կ��� �˷����Ѵ�.
		//�� �������� ���� �����ϴ� �༮���� ���̴°�??�����ϰ� MOVE�ѹ� ������ϳ�??�������� �־���.
		for (int i = 0; i < playerSessionCnt; i++)
		{
			//�޽��� �ۼ�
			msgCreateStar.type = CREATE_STAR;
			msgCreateStar.id = playerSessionArr[i].id;
			msgCreateStar.x = playerSessionArr[i].x;
			msgCreateStar.y = playerSessionArr[i].y;
			wprintf_s(L"playerCnt:%d\n", playerSessionCnt);
			wprintf_s(L"id:%d, x:%d, y:%d\n", msgCreateStar.id, msgCreateStar.x, msgCreateStar.y);
			SendUnicast(&playerSessionArr[playerSessionCnt], (MSG_BASE*)&msgCreateStar, &msgSize);
		}

		playerSessionCnt++;//<<<<<<<<<<<�� �༮�� ��ġ �߿��ϴ�!! ��ε� ĳ��Ʈ�� �ȸԾ���.

		//������ ������ ��ο��� �˷����Ѵ�.
		//�� ���� �޽��� �ۼ�
		msgCreateStar.type = CREATE_STAR;
		msgCreateStar.id = g_ID;
		msgCreateStar.x = INIT_X;
		msgCreateStar.y = INIT_Y;

		//wprintf_s(L"�޽��� �ּ�:%p\n", &msgCreateStar);
		//wprintf_s(L"�޽��� ������:%d\n", msgSize);
		//ù ������ �Ѹ��� �����ϰ� ��κ���(NULL�̸� ��κ���)
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

	////wprintf_s(L"�޽����ּ�:%p\n", msg);
	////wprintf_s(L"�޽��� ������:%d\n", *msgSize);
	//int retSend = 0;
	////wprintf_s(L"���� �޴»��:%d\n", playerSession->clntSock);
	//retSend = send(playerSession->clntSock, (char*)msg, *msgSize, 0);
	//if (retSend == SOCKET_ERROR)
	//	wprintf_s(L"send() error:%d\n", WSAGetLastError());
	////wprintf_s(L"���۰��:%d\n", retSend);
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

	//wprintf_s(L"�޽����ּ�:%p\n", msg);
	//wprintf_s(L"�޽��� ������:%d\n", *msgSize);
	//int retSend = 0;
	//for (int i = 0; i < playerSessionCnt; i++)
	//{
	//	//wprintf_s(L"i:%d\n", i);
	//	//�ּҰ� ��
	//	//wprintf_s(L"�ּҰ� ��:%p\n", playerSession);
	//	//wprintf_s(L"�ּҰ� ��:%p\n", &playerSessionArr[i]);
	//	if (playerSession == &playerSessionArr[i])
	//		continue;
	//	//wprintf_s(L"��ε� �޴»��:%d\n", playerSessionArr[i].clntSock);
	//	retSend = send(playerSessionArr[i].clntSock, (char*)msg, *msgSize, 0);
	//	if (retSend == SOCKET_ERROR)
	//		wprintf_s(L"send() error:%d\n", WSAGetLastError());
	//	//wprintf_s(L"���۰��:%d\n", retSend);
	//}
}

void ReadProc(PlayerSession* playerSession)
{
	char buffer[BUF_SIZE];//msg[max]??����??
	while (1)
	{
		int retRecv = 0;
		retRecv = recv(playerSession->clntSock, buffer, sizeof(buffer), 0);

		if (WSAGetLastError() == WSAEWOULDBLOCK || retRecv == 0 /*|| retRecv == -1*/)
			break;

		if (retRecv == SOCKET_ERROR)
		{
			wprintf_s(L"recv():%d\n", WSAGetLastError());

			//������ �ޱ� �������� ��ο��� ��ε�ĳ��Ʈ�� �� �� �����϶�� ����
			//�޽��� �ۼ�
			MSG_DELETE_STAR msgDeleteStar;
			int msgSize = sizeof(msgDeleteStar);
			msgDeleteStar.type = DELETE_STAR;
			msgDeleteStar.id = playerSession->id;
			//�޽��� ����
			SendBroadcast(playerSession, (MSG_BASE*)&msgDeleteStar, &msgSize);
			//���� ���� ����
			closesocket(playerSession->clntSock);
			//���� �迭���� ����
			for (int i = 0; i < playerSessionCnt; i++)
			{
				if (playerSession->id == playerSessionArr[i].id)
				{
					playerSessionArr[i] = playerSession[playerSessionCnt - 1];
					playerSessionCnt--;
				}
			}
			//read��Ʈ���� ����
			//wprintf_s(L"��Ʈ���� ����:%d\n", playerSession->clntSock);
			FD_CLR(playerSession->clntSock, &readSet);

			break;
		}
		//-----------------------------------------------------------------------------//
		playerSession->recvQ.Enqueue(buffer, retRecv);
		while (1)
		{
			if (playerSession->recvQ.GetUseSize() < 16)
				break;//�ߴ�
			char msg[16];
			int retDeQ = 0;
			retDeQ = playerSession->recvQ.Dequeue(msg, 16);
			if (retDeQ != 16)
				exit(1);//����
			//-----------------------------------------------------------------------------//

			int type = *(int*)msg;
			//wprintf(L"���� �޽��� Ÿ��: %d\n", type);

			switch (type)
			{
			case MOVE_STAR://Ŭ�� ���� ��������
			{
				//���� �޽��� �ؼ�
				MSG_MOVE_STAR* recvMSG = (MSG_MOVE_STAR*)msg;
				//���� �迭�� ��ǥ ���� ����
				for (int i = 0; i < playerSessionCnt; i++)
				{
					if (recvMSG->id == playerSessionArr[i].id)
					{
						playerSessionArr[i].x = recvMSG->x;
						playerSessionArr[i].y = recvMSG->y;
					}
				}

				//�޽��� �ۼ�
				MSG_MOVE_STAR msgMoveStar;
				int msgSize = sizeof(msgMoveStar);
				msgMoveStar.type = MOVE_STAR;
				msgMoveStar.id = playerSession->id;
				msgMoveStar.x = recvMSG->x;
				msgMoveStar.y = recvMSG->y;

				//�� Ŭ�� �����ϰ� ��ε�ĳ����
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
	//ȭ�� �����
	system("cls");

	//�迭 ' '�� �ʱ�ȭ, �������� '\0'
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH - 1; x++)
		{
			printArr[y][x] = L' ';
		}
		printArr[y][WIDTH - 1] = L'\0';
	}

	//�÷��̾�� ��ġ�� * �ֱ�
	for (int i = 0; i < playerSessionCnt; i++)
	{
		printArr[playerSessionArr[i].y][playerSessionArr[i].x] = L'*';
	}

	//ȭ�鿡 ���
	for (int y = 0; y < HEIGHT; y++)
	{
		wprintf_s(L"%s\n", printArr[y]);
	}
}

HANDLE hConsole;
void cs_Initial()
{
	CONSOLE_CURSOR_INFO stConsoleCursor;
	//ȭ���� Ŀ���� �Ⱥ��̰� ����
	stConsoleCursor.bVisible = FALSE;
	stConsoleCursor.dwSize = 1;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(hConsole, &stConsoleCursor);
}