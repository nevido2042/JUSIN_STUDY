#include <locale>
#include <iostream>
#include "CServer.h"

CServer::CServer()
    :m_ServSock(INVALID_SOCKET),
    //m_iSessionCnt(0),
    m_iID(0)
{
    ZeroMemory(&m_tReadSet, sizeof(fd_set));
}

CServer::~CServer()
{
}

bool CServer::Initialize()
{
    setlocale(LC_ALL, "KOREAN");

    // ���� �ʱ�ȭ
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        wprintf_s(L"WSAStartup() error:%d\n", WSAGetLastError());
        return false;
    }
    wprintf_s(L"Success WSAStartup()\n");

    // ���� ���� ����
    m_ServSock = socket(AF_INET, SOCK_STREAM, 0);
    if (m_ServSock == INVALID_SOCKET)
    {
        wprintf_s(L"socket() error:%d\n", WSAGetLastError());
        return false;
    }
    wprintf_s(L"Success socket()\n");

    // �ͺ��ŷ ���� ����
    u_long mode = 1;
    ioctlsocket(m_ServSock, FIONBIO, &mode);

    // ���� �ּ� ����
    SOCKADDR_IN servAdr;
    ZeroMemory(&servAdr, sizeof(servAdr));
    servAdr.sin_family = AF_INET;
    servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAdr.sin_port = htons(_wtoi(PORT));

    // ���ε�
    if (bind(m_ServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
    {
        wprintf_s(L"bind() error:%d\n", WSAGetLastError());
        return false;
    }
    wprintf_s(L"Success bind()\n");

    // ����
    if (listen(m_ServSock, 5) == SOCKET_ERROR)
    {
        wprintf_s(L"listen() error:%d\n", WSAGetLastError());
        return false;
    }
    wprintf_s(L"Success listen()\n");

    wprintf_s(L"���� ���� ����\n");

    return true;
}

bool CServer::Update()
{
    return Network();
}

void CServer::Release()
{
}

bool CServer::Network()
{
    Recieve_Message();
    // �޽��� ���� ó��
    Send_Message();

    return true;
}


void CServer::AcceptProc()
{
    while (true)
    {
        SOCKADDR_IN clntAdr;
        int iAdrSize = sizeof(clntAdr);
        SOCKET clntSock = accept(m_ServSock, (SOCKADDR*)&clntAdr, &iAdrSize);

        if (clntSock == INVALID_SOCKET)
        {
            int iErrCode = WSAGetLastError();
            if (iErrCode == WSAEWOULDBLOCK)
                break;

            wprintf_s(L"accept() error: %d\n", iErrCode);
            return;
        }

        cout << "ID:" << m_iID << " Ŭ���̾�Ʈ ����" << endl;

        int iRandX = (WINCX / 10) + rand() % (WINCX / 2);
        int iRandY = (WINCY / 10) + rand() % (WINCY / 2);

        SESSION* pNewSession = new SESSION(clntAdr, clntSock, m_iID, iRandX, iRandY);
        m_vecSession.push_back(pNewSession);
        //������ �ʱ�ȭ
        m_vecSession.back()->recvQ = CRingBuffer(5000);
        m_vecSession.back()->sendQ = CRingBuffer(5000);

        // Ŭ���̾�Ʈ���� ID ����
        MSG_ALLOC_ID msgAllocID = { ALLOC_ID, m_iID };
        Send_Unicast(m_vecSession.back(), (MSG_BASE*)&msgAllocID, sizeof(msgAllocID));

        //���� �������Դ� ���Ը� �����ϵ���
        MSG_CREATE_PLAYER tMSG = { CREATE_PLAYER, m_iID, m_vecSession.back()->x, m_vecSession.back()->y };
        Send_Broadcast(m_vecSession.back(), (MSG_BASE*)&tMSG, sizeof(tMSG));

        //���Կ��Դ� �ڽ� ������ ��θ� �����϶��ϰ�
        for (size_t i = 0; i < m_vecSession.size(); i++)
        {
            tMSG.id = m_vecSession[i]->id;
            tMSG.x = m_vecSession[i]->x;
            tMSG.y = m_vecSession[i]->y;
            Send_Unicast(m_vecSession.back(), (MSG_BASE*)&tMSG, sizeof(tMSG));
        }

        m_iID++;
    }
}

void CServer::Send_Unicast(SESSION* pSession, const MSG_BASE* pMSG, const int iSize)
{
    int iResult{ 0 };
    iResult = pSession->sendQ.Enqueue((char*)pMSG, iSize);
    if (iResult < iSize)
    {
        wprintf_s(L"sendQ.Enqueue() error\n");
        exit(1);
    }
}

void CServer::Send_Broadcast(SESSION* _pSession, const MSG_BASE* pMSG, const int iSize)
{
    int iResult{ 0 };

    for (SESSION* pSession : m_vecSession)
    {
        if (pSession == _pSession)
            continue;

        iResult = pSession->sendQ.Enqueue((char*)pMSG, iSize);
        if (iResult < iSize)
        {
            wprintf_s(L"sendQ.Enqueue() error\n");
            exit(1);
        }
    }

}

void CServer::Read_Proc(SESSION* _pSession)
{
    //recvQ�� ���̷�Ʈ�� �ȴ´�.
    if (_pSession->recvQ.DirectEnqueueSize() > 0)
    {
        int retRecv = recv(_pSession->clntSock, _pSession->recvQ.GetRearBufferPtr() + 1, _pSession->recvQ.DirectEnqueueSize(), 0);

        if (retRecv == 0)
        {
            Delete_Session(_pSession);
            return;
        }
        if (retRecv == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAEWOULDBLOCK)
                return;

            Delete_Session(_pSession);
            wprintf_s(L"recv() Error:%d(Line:%d)\n", WSAGetLastError(), __LINE__);
            return;
        }

        _pSession->recvQ.MoveRear(retRecv);
    }
    else//���۸� ���� ���ú�ť�� �ȴ´�.
    {
        char Buffer[BUF_SIZE];
        int iResult = recv(_pSession->clntSock, Buffer, sizeof(Buffer), 0);
        int iErrCode = WSAGetLastError();

        if (iResult == 0 || (iResult == SOCKET_ERROR && iErrCode != WSAEWOULDBLOCK))
        {
            if (iErrCode)
            {
                wprintf_s(L"recv() error: %d\n", iErrCode);
            }

            // ���� ����
            FD_CLR(_pSession->clntSock, &m_tReadSet);
            closesocket(_pSession->clntSock);
        }

        _pSession->recvQ.Enqueue(Buffer, iResult);
    }

    while (true)
    {
        if (_pSession->recvQ.GetUseSize() < MSG_SIZE)
            break;//�ߴ�

        char Msg[MSG_SIZE];

        int iResult{ 0 };
        iResult = _pSession->recvQ.Dequeue(Msg, MSG_SIZE);
        if (iResult != MSG_SIZE)
        {
            //����
            exit(1);
        }

        int iType{ 0 };
        if (iResult > 0)
        {
            memcpy(&iType, Msg, sizeof(int));
            Decode_Message(iType, Msg);
        }
    }
}

void CServer::Decode_Message(int iType, char* pMsg)
{
    switch (iType)
    {
    case MOVE_RIGHT_PLAYER:
    {
        MSG_MOVE_RIGHT_PLAYER& recvMSG = (MSG_MOVE_RIGHT_PLAYER&)*pMsg;
        wprintf_s(L"%d: MOVE_RIGHT_PLAYER\n", recvMSG.id);

        MSG_MOVE_RIGHT_PLAYER msgMoveRight;
        msgMoveRight.id = recvMSG.id;

        Send_Broadcast(NULL, (MSG_BASE*)&msgMoveRight, sizeof(MSG_MOVE_RIGHT_PLAYER));
        break;
    }
    case MOVE_LEFT_PLAYER:
    {
        MSG_MOVE_LEFT_PLAYER& recvMSG = (MSG_MOVE_LEFT_PLAYER&)*pMsg;
        wprintf_s(L"%d: MOVE_LEFT_PLAYER\n", recvMSG.id);

        MSG_MOVE_LEFT_PLAYER msgMoveLeft;
        msgMoveLeft.id = recvMSG.id;

        Send_Broadcast(NULL, (MSG_BASE*)&msgMoveLeft, sizeof(MSG_MOVE_LEFT_PLAYER));
        break;
    }
    case STOP_RIGHT_PLAYER:
    {
        MSG_STOP_RIGHT_PLAYER& recvMSG = (MSG_STOP_RIGHT_PLAYER&)*pMsg;
        wprintf_s(L"%d: STOP_RIGHT_PLAYER\n", recvMSG.id);

        MSG_STOP_RIGHT_PLAYER msgStopRight;
        msgStopRight.id = recvMSG.id;

        Send_Broadcast(NULL, (MSG_BASE*)&msgStopRight, sizeof(MSG_STOP_RIGHT_PLAYER));
        break;
    }
    case STOP_LEFT_PLAYER:
    {
        MSG_STOP_LEFT_PLAYER& recvMSG = (MSG_STOP_LEFT_PLAYER&)*pMsg;
        wprintf_s(L"%d: STOP_LEFT_PLAYER\n", recvMSG.id);

        MSG_STOP_LEFT_PLAYER msgStopLeft;
        msgStopLeft.id = recvMSG.id;

        Send_Broadcast(NULL, (MSG_BASE*)&msgStopLeft, sizeof(MSG_STOP_LEFT_PLAYER));
        break;
    }
    case DELETE_PLAYER:
    {
        MSG_DELETE_PLAYER& recvMSG = (MSG_DELETE_PLAYER&)*pMsg;
        wprintf_s(L"%d: DELETE_PLAYER\n", recvMSG.id);

        MSG_DELETE_PLAYER msgDeletePlayer;
        msgDeletePlayer.id = recvMSG.id;
        
        //������ ���̵� ���� �༮�� ������ �����ϰ� �������ߴµ� �׳� ��������

        Send_Broadcast(NULL/*&m_vecSession[msgDeletePlayer.id]*/, (MSG_BASE*)&msgDeletePlayer, sizeof(MSG_DELETE_PLAYER));
        break;
    }
    }
}

void CServer::Recieve_Message()
{
    fd_set tCpySet;
    FD_ZERO(&m_tReadSet);
    FD_SET(m_ServSock, &m_tReadSet);

    // Ÿ�Ӿƿ� ���� (0��, 0����ũ����)
    TIMEVAL tTimeOut = { 0, 0 };

    // Ŭ���̾�Ʈ ������ �б� ���տ� �߰�

    for (SESSION* pSession : m_vecSession)
    {
        FD_SET(pSession->clntSock, &m_tReadSet);
    }

    // ���� ���¸� Ȯ���ϱ� ���� ���纻
    tCpySet = m_tReadSet;

    // select() ȣ��
    int iResult = select(0, &tCpySet, NULL, NULL, &tTimeOut);
    if (iResult == SOCKET_ERROR)
    {
        wprintf_s(L"select() error:%d\n", WSAGetLastError());
        return;  // ������ �߻��ϸ� ����
    }

    // select()�� ���������� �Ϸ�� ���
    if (iResult > 0)
    {
        // ���õ� ���ϵ��� ���������� ó��
        for (u_int i = 0; i < tCpySet.fd_count; i++)
        {
            SOCKET currentSock = tCpySet.fd_array[i];

            // ���� ����(���ο� Ŭ���̾�Ʈ ����) ó��
            if (currentSock == m_ServSock)
            {
                AcceptProc(); // Ŭ���̾�Ʈ ���� ó��
            }
            else
            {
                for (auto it = m_vecSession.begin(); it != m_vecSession.end(); )
                {
                    if ((*it)->clntSock == currentSock)
                    {
                        // ������ ���� �� ���� ���� Ȯ��
                        char buffer[1];
                        int result = recv(currentSock, buffer, sizeof(buffer), MSG_PEEK);
                        if (result <= 0) // ���� ����Ǿ��ų� ���� �߻�
                        {
                            MSG_DELETE_PLAYER tMSG;
                            tMSG.type = DELETE_PLAYER;
                            tMSG.id = (*it)->id;
                            Send_Broadcast(NULL, (MSG_BASE*)&tMSG, sizeof(MSG_BASE));

                            // Ŭ���̾�Ʈ ���� ���� ó��
                            closesocket((*it)->clntSock);

                            // ���� ����
                            it = m_vecSession.erase(it); // erase�� ������ ���� ����� iterator�� ��ȯ��
                        }
                        else
                        {
                            Read_Proc(*it);
                            ++it;
                        }
                    }
                    else
                    {
                        ++it;
                    }
                }
            }
        }
    }
}

void CServer::Send_Message()
{
    fd_set writeSet;
    FD_ZERO(&writeSet);
    fd_set cpySet;

    //�÷��̾� ���ǵ��� sendQ ��뿩�θ� Ȯ���ϰ� ��Ʈ�� �ִ´�.
    for (size_t i = 0; i < m_vecSession.size(); i++)
    {
        if (m_vecSession[i]->sendQ.GetUseSize() > 0)
            FD_SET(m_vecSession[i]->clntSock, &writeSet);
    }

    TIMEVAL tTimeOut = { 0, 0 };

    cpySet = writeSet;

    int iResult = select(0, NULL, &cpySet, NULL, &tTimeOut);
    if (iResult > 0)
    {
        char Buffer[BUF_SIZE];
        for (u_int i = 0; i < writeSet.fd_count; i++)
        {
            if (FD_ISSET(writeSet.fd_array[i], &cpySet))
            {
                for (size_t j = 0; j < m_vecSession.size(); j++)
                {
                    if (m_vecSession[j]->clntSock != writeSet.fd_array[j])
                        continue;

                    int iPeek{};
                    iPeek = m_vecSession[j]->sendQ.Peek(Buffer, m_vecSession[j]->sendQ.GetUseSize());

                    int iSend{};
                    iSend = send(m_vecSession[j]->clntSock, Buffer, iPeek, 0);
                    if (iSend == SOCKET_ERROR)
                        wprintf_s(L"send() error:%d\n", WSAGetLastError());

                    m_vecSession[j]->sendQ.MoveFront(iSend);
                }
            }
        }
    }
}

void CServer::Delete_Session(SESSION* _pSession)
{
    //Ŭ���� ����
    closesocket(_pSession->clntSock);

    //��ε� ĳ��Ʈ ĳ���� ����
    MSG_DELETE_PLAYER tMsg;
    tMsg.id = _pSession->id;
    tMsg.type = DELETE_PLAYER;
    wprintf_s(L"id:%d Disconnect\n", _pSession->id);
    Send_Broadcast(_pSession, (MSG_BASE*)&tMsg, sizeof(MSG_BASE));

    //���� ����
    for (auto iter = m_vecSession.begin(); iter != m_vecSession.end();)
    {
        if ((*iter)->id == _pSession->id)
        {
            iter = m_vecSession.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}
