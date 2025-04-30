#include <locale>
#include <iostream>
#include "Define.h"
#include "CServer.h"
#include "PacketHandler.h"

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
	for (size_t i = 0; i < m_vecSession.size(); i++)
	{
		closesocket(m_vecSession[i]->Get_Socket());
		delete m_vecSession[i];
	}
	m_vecSession.clear();
	closesocket(m_ServSock);
	WSACleanup();
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

        CSession* pNewSession = new CSession(clntAdr, clntSock, m_iID, iRandX, iRandY);
        //������ �ʱ�ȭ
        /*pNewSession->m_RecvQ = CRingBuffer(5000);
        pNewSession->m_SendQ = CRingBuffer(5000);*/

        //���� �ڱ� ĳ�� ����
        CPacketHandler::SC_CreateMyCharacter(&m_Packet,
            pNewSession->Get_SessionInfo().iID,
            pNewSession->Get_SessionInfo().iX,
            pNewSession->Get_SessionInfo().iY
        );
        Send_Unicast(pNewSession, (_byte*)m_Packet.GetBufferPtr(), m_Packet.GetDataSize());

        //���� �Ѹ���
        CPacketHandler::SC_CreateOtherCharacter(&m_Packet,
            pNewSession->Get_SessionInfo().iID,
            pNewSession->Get_SessionInfo().iX,
            pNewSession->Get_SessionInfo().iY
        );
        Send_Broadcast(pNewSession, (_byte*)m_Packet.GetBufferPtr(), m_Packet.GetDataSize());

        //���Կ��� ���� ���� �Ѹ���
        for (CSession* pSession : m_vecSession)
        {
            //���� �Ѹ���
            CPacketHandler::SC_CreateOtherCharacter(&m_Packet,
                pSession->Get_SessionInfo().iID,
                pSession->Get_SessionInfo().iX,
                pSession->Get_SessionInfo().iY
            );
            Send_Unicast(pNewSession, (_byte*)m_Packet.GetBufferPtr(), m_Packet.GetDataSize());


        }

        m_vecSession.push_back(pNewSession);
        m_iID++;
    }
}

void CServer::Send_Unicast(CSession* pSession, const _byte* pMSG, const int iSize)
{
    int iResult{ 0 };
    iResult = pSession->Get_SendQ().Enqueue((_byte*)pMSG, iSize);
    if (iResult < iSize)
    {
        wprintf_s(L"sendQ.Enqueue() error\n");
        exit(1);
    }

    m_Packet.Clear();
}

void CServer::Send_Broadcast(CSession* _pSession, const _byte* pMSG, const int iSize)
{
    int iResult{ 0 };

    for (CSession* pSession : m_vecSession)
    {
        if (pSession == _pSession)
            continue;

        iResult = pSession->Get_SendQ().Enqueue((_byte*)pMSG, iSize);
        if (iResult < iSize)
        {
            wprintf_s(L"sendQ.Enqueue() error\n");
            exit(1);
        }
    }

    m_Packet.Clear();
}

void CServer::Read_Proc(CSession* _pSession)
{
    //recvQ�� ���̷�Ʈ�� �ȴ´�.
    if (_pSession->Get_RecvQ().DirectEnqueueSize() > 0)
    {
        int retRecv = recv(_pSession->Get_Socket(), _pSession->Get_RecvQ().GetRearBufferPtr() + 1, _pSession->Get_RecvQ().DirectEnqueueSize(), 0);

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

        _pSession->Get_RecvQ().MoveRear(retRecv);
    }
    else//���۸� ���� ���ú�ť�� �ȴ´�.
    {
        _byte Buffer[BUF_SIZE];
        int iResult = recv(_pSession->Get_Socket(), Buffer, sizeof(Buffer), 0);
        int iErrCode = WSAGetLastError();

        if (iResult == 0 || (iResult == SOCKET_ERROR && iErrCode != WSAEWOULDBLOCK))
        {
            if (iErrCode)
            {
                wprintf_s(L"recv() error: %d\n", iErrCode);
            }

            // ���� ����
            FD_CLR(_pSession->Get_Socket(), &m_tReadSet);
            closesocket(_pSession->Get_Socket());
        }

        _pSession->Get_RecvQ().Enqueue(Buffer, iResult);
    }

    while (1)
    {
        //�޽��� �м�
        tagPACKET_HEADER tHeader;

        //����� �� �̴� ���� �ۿ� �ȵ��Դٸ�
        if (sizeof(tHeader) > _pSession->Get_RecvQ().GetUseSize())
        {
            break;
        }

        int retPeek = _pSession->Get_RecvQ().Peek((_byte*)&tHeader, sizeof(tHeader));
        if (retPeek != sizeof(tHeader))
        {
            wprintf_s(L"Peek() Error:%d\n", retPeek);
            exit(1);
        }
        if (tHeader.byCode != PACKET_CODE)
        {
            wprintf_s(L"BYTEbyCode Error:%d\n", tHeader.byCode);
            exit(1);
        }
        if (tHeader.bySize + sizeof(tHeader) > (size_t)_pSession->Get_RecvQ().GetUseSize())
        {
            break;
        }

        _pSession->Get_RecvQ().MoveFront(sizeof(tHeader));
        Decode_Message(tHeader, _pSession);
    }
}

void CServer::Decode_Message(const tagPACKET_HEADER& _Header, CSession* _pSession)
{
    //_pSession->Recive((_byte*)m_Packet.GetBufferPtr(), _Header.bySize);
    int iResult = _pSession->Recive_Data(m_Packet, _Header.bySize);//_pSession->m_RecvQ.Dequeue((char*)m_Packet.GetBufferPtr(), _Header.bySize);
    if (iResult != _Header.bySize)
    {
        wprintf_s(L"Dequeue() Error:%d\n", iResult);
        exit(1);
    }
    m_Packet.Move_WritePos(iResult);

    switch (_Header.byType)
    {
    case PACKET_CS_DELETE_MY_CHARACTER:
    {
        CPacketHandler::SC_DeleteCharacter(&m_Packet, _pSession->Get_SessionInfo().iID);

        wprintf_s(L"ID: %d, ĳ���� ����\n", _pSession->Get_SessionInfo().iID);

        Send_Broadcast(_pSession, (_byte*)m_Packet.GetBufferPtr(), m_Packet.GetDataSize());

        break;
    }
    }

    //switch (iType)
    //{
    //case MOVE_RIGHT_PLAYER:
    //{
    //    MSG_MOVE_RIGHT_PLAYER& recvMSG = (MSG_MOVE_RIGHT_PLAYER&)*pMsg;
    //    wprintf_s(L"%d: MOVE_RIGHT_PLAYER\n", recvMSG.id);

    //    MSG_MOVE_RIGHT_PLAYER msgMoveRight;
    //    msgMoveRight.id = recvMSG.id;

    //    Send_Broadcast(NULL, (_byte*)&msgMoveRight, sizeof(MSG_MOVE_RIGHT_PLAYER));
    //    break;
    //}
    //case MOVE_LEFT_PLAYER:
    //{
    //    MSG_MOVE_LEFT_PLAYER& recvMSG = (MSG_MOVE_LEFT_PLAYER&)*pMsg;
    //    wprintf_s(L"%d: MOVE_LEFT_PLAYER\n", recvMSG.id);

    //    MSG_MOVE_LEFT_PLAYER msgMoveLeft;
    //    msgMoveLeft.id = recvMSG.id;

    //    Send_Broadcast(NULL, (_byte*)&msgMoveLeft, sizeof(MSG_MOVE_LEFT_PLAYER));
    //    break;
    //}
    //case STOP_RIGHT_PLAYER:
    //{
    //    MSG_STOP_RIGHT_PLAYER& recvMSG = (MSG_STOP_RIGHT_PLAYER&)*pMsg;
    //    wprintf_s(L"%d: STOP_RIGHT_PLAYER\n", recvMSG.id);

    //    MSG_STOP_RIGHT_PLAYER msgStopRight;
    //    msgStopRight.id = recvMSG.id;

    //    Send_Broadcast(NULL, (_byte*)&msgStopRight, sizeof(MSG_STOP_RIGHT_PLAYER));
    //    break;
    //}
    //case STOP_LEFT_PLAYER:
    //{
    //    MSG_STOP_LEFT_PLAYER& recvMSG = (MSG_STOP_LEFT_PLAYER&)*pMsg;
    //    wprintf_s(L"%d: STOP_LEFT_PLAYER\n", recvMSG.id);

    //    MSG_STOP_LEFT_PLAYER msgStopLeft;
    //    msgStopLeft.id = recvMSG.id;

    //    Send_Broadcast(NULL, (_byte*)&msgStopLeft, sizeof(MSG_STOP_LEFT_PLAYER));
    //    break;
    //}
    //case DELETE_PLAYER:
    //{
    //    //MSG_DELETE_PLAYER& recvMSG = (MSG_DELETE_PLAYER&)*pMsg;
    //    //wprintf_s(L"%d: DELETE_PLAYER\n", recvMSG.id);

    //    //MSG_DELETE_PLAYER msgDeletePlayer;
    //    //msgDeletePlayer.id = recvMSG.id;
    //    //
    //    ////������ ���̵� ���� �༮�� ������ �����ϰ� �������ߴµ� �׳� ��������

    //    //Send_Broadcast(NULL/*&m_vecSession[msgDeletePlayer.id]*/, (_byte*)&msgDeletePlayer, sizeof(MSG_DELETE_PLAYER));
    //    break;
    //}
    //}
}

void CServer::Recieve_Message()
{
    fd_set tCpySet;
    FD_ZERO(&m_tReadSet);
    FD_SET(m_ServSock, &m_tReadSet);

    // Ÿ�Ӿƿ� ���� (0��, 0����ũ����)
    TIMEVAL tTimeOut = { 0, 0 };

    // Ŭ���̾�Ʈ ������ �б� ���տ� �߰�

    for (CSession* pSession : m_vecSession)
    {
        FD_SET(pSession->Get_Socket(), &m_tReadSet);
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
                    if ((*it)->Get_Socket() == currentSock)
                    {
                        // ������ ���� �� ���� ���� Ȯ��
                        _byte buffer[1];
                        int result = recv(currentSock, buffer, sizeof(buffer), MSG_PEEK);
                        if (result <= 0) // ���� ����Ǿ��ų� ���� �߻�
                        {
                            /*MSG_DELETE_PLAYER tMSG;
                            tMSG.type = DELETE_PLAYER;
                            tMSG.id = (*it)->iID;
                            Send_Broadcast(NULL, (_byte*)&tMSG, sizeof(MSG_BASE));*/

                            // Ŭ���̾�Ʈ ���� ���� ó��
                            closesocket((*it)->Get_Socket());
                            Safe_Delete(*it);
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
        if (m_vecSession[i]->Get_SendQ().GetUseSize() > 0)
            FD_SET(m_vecSession[i]->Get_Socket(), &writeSet);
    }

    TIMEVAL tTimeOut = { 0, 0 };

    cpySet = writeSet;

    int iResult = select(0, NULL, &cpySet, NULL, &tTimeOut);
    if (iResult > 0)
    {
        _byte Buffer[BUF_SIZE];

        for (u_int i = 0; i < writeSet.fd_count; i++)
        {
            SOCKET writeSock = writeSet.fd_array[i]; // ���� write ���� ����

            // ���Ϳ��� �ش� ������ ���� ������ ã��
            for (size_t j = 0; j < m_vecSession.size(); j++)
            {
                if (m_vecSession[j]->Get_Socket() == writeSock)
                {
                    // ���� ������ ť���� ���� ������ ��������
                    int iPeek = m_vecSession[j]->Get_SendQ().Peek(Buffer, m_vecSession[j]->Get_SendQ().GetUseSize());
                    if (iPeek <= 0) // ���� �����Ͱ� ����
                        continue;

                    // ������ ����
                    int iSend = send(writeSock, Buffer, iPeek, 0);
                    if (iSend == SOCKET_ERROR)
                    {
                        wprintf_s(L"ID:%d, send() error:%d\n", m_vecSession[j]->Get_SessionInfo().iID, WSAGetLastError());
                        continue;
                    }

                    // ���۵� ��ŭ ť�� �����͸� �̵�
                    m_vecSession[j]->Get_SendQ().MoveFront(iSend);
                    // wprintf_s(L"To IndexSock: %d, Send Byte: %d\n", i, iSend);

                    break; // �ش� writeSock�� ���� ó���� �������Ƿ� ���� ���� ����
                }
            }
        }
    }

}

void CServer::Delete_Session(CSession* _pSession)
{
    //Ŭ���� ����
    closesocket(_pSession->Get_Socket());

    ////��ε� ĳ��Ʈ ĳ���� ����
    //MSG_DELETE_PLAYER tMsg;
    //tMsg.id = _pSession->iID;
    //tMsg.type = DELETE_PLAYER;
    //wprintf_s(L"id:%d Disconnect\n", _pSession->iID);
    //Send_Broadcast(_pSession, (_byte*)&tMsg, sizeof(MSG_BASE));

    //���� ����
    for (auto iter = m_vecSession.begin(); iter != m_vecSession.end();)
    {
        if ((*iter)->Get_SessionInfo().iID == _pSession->Get_SessionInfo().iID)
        {
            iter = m_vecSession.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}