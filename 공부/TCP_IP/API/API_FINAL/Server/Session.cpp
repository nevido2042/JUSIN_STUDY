#include "Session.h"

CSession::CSession(SOCKADDR_IN _clntAdr, SOCKET _clntSock, int _id, int _x, int _y)
	:m_ClientAddress(_clntAdr), m_ClientSocket(_clntSock), m_iID(_id)
{
	//���Ŀ� �÷��̾� ������ ���� �����ϴ� Ŭ������ ����� ���� ���� �� ����
	iX = _x;
	iY = _y;
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

int CSession::Recive_Data(CPacket& pPakcet, int Size)
{
	return m_RecvQ.Dequeue(pPakcet.GetBufferPtr(), Size);
}
