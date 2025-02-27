#include"Packet.h"
#include<stdio.h>
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
	free(m_pAlloc);
}

void CPacket::Clear()
{
	m_Front = m_pAlloc;  // ������ ���� ��ġ�� �ʱ�ȭ
	m_Rear = m_pAlloc;   // ������ ���� ��ġ�� �ʱ�ȭ
	//rear = front;
	m_iDataSize = 0;
}

int CPacket::GetBufferSize()
{
	return m_iBufferSize;
}

int CPacket::GetDataSize()
{
	return m_Rear - m_Front;
}

_byte* CPacket::GetBufferPtr()
{
	return m_pAlloc;
}

int CPacket::MoveReadPos(int iSize)
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

int CPacket::MoveWritePos(int iSize)
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

void CPacket::Enqueue(_byte* pData, int iSize)
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

	m_iDataSize = m_Rear - m_Front;
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

	m_iDataSize = m_Rear - m_Front;
}

int	CPacket::GetData(_byte* pDest, int iSize)
{
	Dequeue(pDest, iSize);
	return iSize;
}

int	CPacket::PutData(_byte* pSrc, int iSize)
{
	Enqueue(pSrc, iSize);
	return iSize;
}

void CPacket::UpdateHeaderSize(int iSize) 
{
	// ��Ŷ ũ�� ������ ���� 2��° ����Ʈ ��ġ
	//*(GetBufferPtr() + 1) = static_cast<char>(packetSize);

	// PACKET_HEADER ����ü�� ������ ó���� ��ġ�Ѵٰ� ����
	tagPACKET_HEADER* pHeader = reinterpret_cast<tagPACKET_HEADER*>(GetBufferPtr());
	pHeader->bySize = static_cast<_byte>(iSize);
}