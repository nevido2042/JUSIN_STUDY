#include "PacketHandler.h"
#include "Server.h"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>


CServer::CServer()
{
    ZeroMemory(&m_tReadSet, sizeof(fd_set));
}

CServer::~CServer()
{
}

_bool CServer::Initialize()
{
    m_iPort = Load_Config_File(TEXT("../bin/config.txt"));

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
    servAdr.sin_port = htons(m_iPort);

    // ���ε�
    if (::bind(m_ServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
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

_bool CServer::Update()
{
    // ESC�� S�� ���ÿ� ������ ���� ����
    if ((GetAsyncKeyState(VK_ESCAPE) & 0x8000) &&
        (GetAsyncKeyState('S') & 0x8000))
    {
        return false;
    }

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

_bool CServer::Network()
{
    Recieve_Message();
    Send_Message();
    Delete_Dead_Session();

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

        int iRandX{ rand() % 3 };
        int iRandZ{ rand() % 3 };

        _float3 Position{ static_cast<_float>(iRandX) , 0.f, static_cast<_float>(iRandZ) };

        CSession* pNewSession = new CSession(clntAdr, clntSock, m_iID, Position);
        //������ �ʱ�ȭ
        /*pNewSession->m_RecvQ = CRingBuffer(5000);
        pNewSession->m_SendQ = CRingBuffer(5000);*/

        //���� �ڱ� ĳ�� ����
        CPacketHandler::mp_SC_CreateMyCharacter(&m_Packet,
            pNewSession->Get_SessionInfo().iID,
            pNewSession->Get_SessionInfo().Position
        );
        Send_Unicast(pNewSession, (_byte*)m_Packet.Get_BufferPtr(), m_Packet.Get_DataSize());

        //���� �Ѹ���
        CPacketHandler::mp_SC_CreateOtherCharacter(&m_Packet,
            pNewSession->Get_SessionInfo().iID,
            pNewSession->Get_SessionInfo().Position
        );
        Send_Broadcast(pNewSession, (_byte*)m_Packet.Get_BufferPtr(), m_Packet.Get_DataSize());

        //���Կ��� ���� ���� �Ѹ���
        for (CSession* pSession : m_vecSession)
        {
            //���� �Ѹ���
            CPacketHandler::mp_SC_CreateOtherCharacter(&m_Packet,
                pSession->Get_SessionInfo().iID,
                pSession->Get_SessionInfo().Position
            );
            Send_Unicast(pNewSession, (_byte*)m_Packet.Get_BufferPtr(), m_Packet.Get_DataSize());
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
        int retRecv = recv(_pSession->Get_Socket(), (char*)_pSession->Get_RecvQ().Get_Rear() + 1, _pSession->Get_RecvQ().DirectEnqueueSize(), 0);

        if (retRecv == 0)
        {
            Set_Session_Dead(_pSession);
            return;
        }
        if (retRecv == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAEWOULDBLOCK)
                return;

            Set_Session_Dead(_pSession);
            wprintf_s(L"recv() Error:%d(Line:%d)\n", WSAGetLastError(), __LINE__);
            return;
        }

        _pSession->Get_RecvQ().Move_Rear(retRecv);
    }
    else//���۸� ���� ���ú�ť�� �ȴ´�.
    {
        _byte Buffer[BUF_SIZE];
        int iResult = recv(_pSession->Get_Socket(), (char*)Buffer, sizeof(Buffer), 0);
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

    while (true)
    {
        //�޽��� �м�
        tagPACKET_HEADER tHeader;

        //����� �� �̴� ���� �ۿ� �ȵ��Դٸ�
        if (sizeof(tHeader) > _pSession->Get_RecvQ().Get_UseSize())
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
            //Delete_Session(_pSession);
        }
        if (tHeader.bySize + sizeof(tHeader) > (size_t)_pSession->Get_RecvQ().Get_UseSize())
        {
            break;
        }

        _pSession->Get_RecvQ().Move_Front(sizeof(tHeader));
        Decode_Message(tHeader, _pSession);
    }
}

void CServer::Decode_Message(const tagPACKET_HEADER& _Header, CSession* _pSession)
{
    int iResult = _pSession->Receive_Data(m_Packet, _Header.bySize);
    if (iResult != _Header.bySize)
    {
        wprintf_s(L"Dequeue() Error:%d\n", iResult);
        exit(1);
    }
    m_Packet.Move_WritePos(iResult);

    switch (_Header.byType)
    {
	case ENUM_CLASS(PacketType::CS_PING):
        cout << "CS_PING" << endl;
		CPacketHandler::mp_SC_Ping(&m_Packet);
        Send_Unicast(_pSession, (_byte*)m_Packet.Get_BufferPtr(), m_Packet.Get_DataSize());
		break;

    case ENUM_CLASS(PacketType::CS_POSITION):

        _float3 vPos;
        CPacketHandler::net_Position(&m_Packet, vPos);

        cout << vPos.x << ' ' << vPos.y << ' ' << vPos.z << endl;
        break;
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
				Check_Session_State(currentSock);
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
        if (m_vecSession[i]->Get_SendQ().Get_UseSize() > 0)
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
                    int iPeek = m_vecSession[j]->Get_SendQ().Peek(Buffer, m_vecSession[j]->Get_SendQ().Get_UseSize());
                    if (iPeek <= 0) // ���� �����Ͱ� ����
                        continue;

                    // ������ ����
                    int iSend = send(writeSock, (char*)Buffer, iPeek, 0);
                    if (iSend == SOCKET_ERROR)
                    {
                        wprintf_s(L"ID:%d, send() error:%d\n", m_vecSession[j]->Get_SessionInfo().iID, WSAGetLastError());
                        continue;
                    }

                    // ���۵� ��ŭ ť�� �����͸� �̵�
                    m_vecSession[j]->Get_SendQ().Move_Front(iSend);
                    // wprintf_s(L"To IndexSock: %d, Send Byte: %d\n", i, iSend);

                    break; // �ش� writeSock�� ���� ó���� �������Ƿ� ���� ���� ����
                }
            }
        }
    }

}

void CServer::Delete_Dead_Session()
{
    //���� ����
    for (auto iter = m_vecSession.begin(); iter != m_vecSession.end();)
    {
        if ((*iter)->Get_IsDead())
        {
            //Ŭ���� ����
            closesocket((*iter)->Get_Socket());
            iter = m_vecSession.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

void CServer::Check_Session_State(SOCKET sock)
{
    for (auto iter = m_vecSession.begin(); iter != m_vecSession.end(); )
    {
        if ((*iter)->Get_Socket() == sock)
        {
            // ������ ���� �� ���� ���� Ȯ��
            _byte buffer[1];
            int result = recv(sock, (char*)buffer, sizeof(buffer), MSG_PEEK);
            if (result <= 0) // ���� ����Ǿ��ų� ���� �߻�
            {
                wprintf_s(L"Ŭ���̾�Ʈ ���� ID:%d, WSAGetLastError:%d\n", (*iter)->Get_SessionInfo().iID, WSAGetLastError());
                CPacketHandler::mp_SC_DeleteCharacter(&m_Packet, (*iter)->Get_SessionInfo().iID);
                Send_Broadcast(*iter, m_Packet.Get_BufferPtr(), m_Packet.Get_DataSize());

                // Ŭ���̾�Ʈ ���� ���� ó��
                closesocket((*iter)->Get_Socket());
                Safe_Delete(*iter);
                // ���� ����
                iter = m_vecSession.erase(iter); // erase�� ������ ���� ����� iterator�� ��ȯ��
            }
            else
            {
                Read_Proc(*iter);
                ++iter;
            }
        }
        else
        {
            ++iter;
        }
    }
}

void CServer::Set_Session_Dead(CSession* _pSession)
{
    _pSession->Set_IsDead();
}

int CServer::Load_Config_File(const wstring& filename)
{
    wifstream inFile(filename);
    if (!inFile.is_open())
    {
        cout << "Can't Open ServerConfigFile" << endl;
        return 2042;
    }

    // ������ ù �ٿ��� ��Ʈ ��ȣ �б�
    wstring PortLine;
    getline(inFile, PortLine);
    wstringstream wss(PortLine);
    int iPort;
    wss >> iPort;
    return iPort;
}
