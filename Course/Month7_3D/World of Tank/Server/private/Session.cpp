#include "Session.h"

CSession::CSession(SOCKADDR_IN _clntAdr, SOCKET _clntSock, _int _id, _float3 _Position)
	:m_ClientAddress(_clntAdr), m_ClientSocket(_clntSock)
{
	//���Ŀ� �÷��̾� ������ ���� �����ϴ� Ŭ������ ����� ���� ���� �� ����
	m_tSessionInfo.iID = _id;
	m_tSessionInfo.vPosition = _Position;
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
	return m_RecvQ.Dequeue((_byte*)pPakcet.Get_BufferPtr(), Size);
}

