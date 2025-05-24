#include "Session.h"

CSession::CSession(SOCKADDR_IN _clntAdr, SOCKET _clntSock, _int _id, _float3 _Position)
	:m_ClientAddress(_clntAdr), m_ClientSocket(_clntSock)
{
	//추후에 플레이어 정보를 따로 저장하는 클래스를 만드는 것이 좋을 것 같음
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

