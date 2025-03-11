#include "Session.h"

CSession::CSession(SOCKADDR_IN _clntAdr, SOCKET _clntSock, int _id, int _x, int _y)
	:m_ClientAddress(_clntAdr), m_ClientSocket(_clntSock)
{
	//���Ŀ� �÷��̾� ������ ���� �����ϴ� Ŭ������ ����� ���� ���� �� ����
	m_tSessionInfo.iID = _id;
	m_tSessionInfo.iX = _x;
	m_tSessionInfo.iY = _y;
}

CSession::~CSession()
{
	Release();
}

void CSession::Initialize()
{
}

void CSession::Release()
{
	m_RecvQ.Release();
	m_SendQ.Release();
}

int CSession::Receive_Data(CPacket& pPakcet, int Size)
{
	return m_RecvQ.Dequeue((_BYTE*)pPakcet.Get_BufferPtr(), Size);
}
