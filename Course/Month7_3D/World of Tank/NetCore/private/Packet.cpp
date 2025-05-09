
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
	m_Front = m_pAlloc;  // ������ ���� ��ġ�� �ʱ�ȭ
	m_Rear = m_pAlloc;   // ������ ���� ��ġ�� �ʱ�ȭ
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
	//rear�� �Ѿ�� �ȵȴ�.
	if (m_Front + iSize > m_Rear)
	{
		wprintf_s(L"MoveReadPos() Error:front + iSize > rear\n");
		return -1;
	}
	//������ ���͵� �ȵȴ�.
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
	//rear�� �Ҵ� �������� �Ѿ�� �ȵȴ�.
	if (m_Rear + iSize > m_pAllocEnd)
	{
		wprintf_s(L"MoveWritePos() Error: rear + iSize > allocEndPtr\n");
		return -1;
	}
	//������ ���͵� �ȵȴ�.
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

	//rear ������ �����͸� �ְ�
	//rear++
	//iSize��ŭ �ݺ�
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

	//�����Ϳ� front���� �ְ�
	//front++
	//iSize��ŭ �ݺ�
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
	// ��Ŷ ũ�� ������ ���� 2��° ����Ʈ ��ġ
	//*(GetBufferPtr() + 1) = static_cast<char>(packetSize);

	// PACKET_HEADER ����ü�� ������ ó���� ��ġ�Ѵٰ� ����
	PACKET_HEADER* pHeader = reinterpret_cast<PACKET_HEADER*>(Get_BufferPtr());
	pHeader->bySize = static_cast<_byte>(iSize);
}