
#include"Packet.h"
#include "PacketType.h"

CPacket::CPacket()
{
	m_pAlloc = (_byte*)malloc(eBUFFER_DEFAULT + 1);
	m_Front = m_pAlloc;
	m_Rear = m_pAlloc;
	m_iBufferSize = eBUFFER_DEFAULT;
	m_pAllocEnd = m_pAlloc + eBUFFER_DEFAULT;

	if (m_pAlloc == nullptr)
		wprintf_s(L"CPacket() Error: alloc nullptr\n");
}
CPacket::CPacket(int iSize)
{
	m_pAlloc = (_byte*)malloc(iSize + 1);
	m_Front = m_pAlloc;
	m_Rear = m_pAlloc;
	m_iBufferSize = iSize;
	m_pAllocEnd = m_pAlloc + iSize;

	if (m_pAlloc == nullptr)
		wprintf_s(L"CPacket() Error: alloc nullptr\n");
}

CPacket::~CPacket()
{
	if (m_pAlloc)
	{
		free(m_pAlloc);
		m_pAlloc = nullptr;
	}

}

void CPacket::Clear()
{
	m_Front = m_pAlloc;  // 버퍼의 시작 위치로 초기화
	m_Rear = m_pAlloc;   // 버퍼의 시작 위치로 초기화
	//rear = front;
	m_iDataSize = 0;
}

int CPacket::Get_BufferSize()
{
	return m_iBufferSize;
}

int CPacket::Get_DataSize()
{
	return static_cast<int>(m_Rear - m_Front);
}

_byte* CPacket::Get_BufferPtr()
{
	return m_pAlloc;
}

int CPacket::Move_ReadPos(int iSize)
{
	//rear을 넘어가면 안된다.
	if (m_Front + iSize > m_Rear)
	{
		wprintf_s(L"MoveReadPos() Error:front + iSize > rear\n");
		return -1;
	}
	//음수가 들어와도 안된다.
	if (iSize < 0)
	{
		wprintf_s(L"MoveReadPos() Error:iSize < 0 \n");
		return -1;
	}

	m_Front += iSize;
	return iSize;
}

int CPacket::Move_WritePos(int iSize)
{
	//rear가 할당 끝지점을 넘어서면 안된다.
	if (m_Rear + iSize > m_pAllocEnd)
	{
		wprintf_s(L"MoveWritePos() Error: rear + iSize > allocEndPtr\n");
		return -1;
	}
	//음수가 들어와도 안된다.
	if (iSize < 0)
	{
		wprintf_s(L"MoveReadPos() Error:iSize < 0\n");
		return -1;
	}
	m_Rear += iSize;
	return iSize;
}

void CPacket::Enqueue(const _byte* pData, int iSize)
{
	//rear+size > allocEndPtr?
	if (m_Rear + iSize > m_pAllocEnd)
	{
		wprintf_s(L"Enqueue() Error:rear+size > allocEndPtr\n");
		return;
	}

	//rear 지점에 데이터를 넣고
	//rear++
	//iSize만큼 반복
	for (int i = 0; i < iSize; i++)
	{
		*m_Rear = *pData++;
		m_Rear++;
	}

	m_iDataSize = static_cast<int>(m_Rear - m_Front);
}

void CPacket::Dequeue(_byte* pBuf, int iSize)
{
	//front + size > rear?
	if (m_Front + iSize > m_Rear)
	{
		wprintf_s(L"Dequeue() Error:front+size > rear\n");
		return;
	}

	//데이터에 front값을 넣고
	//front++
	//iSize만큼 반복
	for (int i = 0; i < iSize; i++)
	{
		*pBuf++ = *m_Front;
		m_Front++;
	}

	m_iDataSize = static_cast<int>(m_Rear - m_Front);
}

int	CPacket::Get_Data(_byte* pDest, int iSize)
{
	Dequeue(pDest, iSize);
	return iSize;
}

int	CPacket::Put_Data(_byte* pSrc, int iSize)
{
	Enqueue(pSrc, iSize);
	return iSize;
}

void CPacket::Update_HeaderSize(int iSize) 
{
	// 패킷 크기 정보는 보통 2번째 바이트 위치
	//*(GetBufferPtr() + 1) = static_cast<char>(packetSize);

	// PACKET_HEADER 구조체를 버퍼의 처음에 위치한다고 가정
	PACKET_HEADER* pHeader = reinterpret_cast<PACKET_HEADER*>(Get_BufferPtr());
	pHeader->bySize = static_cast<_byte>(iSize);
}