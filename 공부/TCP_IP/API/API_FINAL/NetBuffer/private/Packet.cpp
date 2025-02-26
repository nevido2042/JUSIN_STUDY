#include"Packet.h"
#include<stdio.h>
CPacket::CPacket()
{
	allocPtr = (char*)malloc(eBUFFER_DEFAULT + 1);
	front = allocPtr;
	rear = allocPtr;
	m_iBufferSize = eBUFFER_DEFAULT;
	allocEndPtr = allocPtr + eBUFFER_DEFAULT;

	if (allocPtr == nullptr)
		wprintf_s(L"CPacket() Error: alloc nullptr\n");
}
CPacket::CPacket(int iBufferSize)
{
	allocPtr = (char*)malloc(iBufferSize + 1);
	front = allocPtr;
	rear = allocPtr;
	m_iBufferSize = iBufferSize;
	allocEndPtr = allocPtr + iBufferSize;

	if (allocPtr == nullptr)
		wprintf_s(L"CPacket() Error: alloc nullptr\n");
}

CPacket::~CPacket()
{
	free(allocPtr);
}

void CPacket::Clear()
{
	front = allocPtr;  // ������ ���� ��ġ�� �ʱ�ȭ
	rear = allocPtr;   // ������ ���� ��ġ�� �ʱ�ȭ
	//rear = front;
}

int CPacket::GetBufferSize()
{
	return m_iBufferSize;
}

int CPacket::GetDataSize()
{
	return rear - front;
}

char* CPacket::GetBufferPtr()
{
	return allocPtr;
}

int CPacket::MoveReadPos(int iSize)
{
	//rear�� �Ѿ�� �ȵȴ�.
	if (front + iSize > rear)
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

	front += iSize;
	return iSize;
}

int CPacket::MoveWritePos(int iSize)
{
	//rear�� �Ҵ� �������� �Ѿ�� �ȵȴ�.
	if (rear + iSize > allocEndPtr)
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
	rear += iSize;
	return iSize;
}

void CPacket::Enqueue(char* value, int size)
{
	//rear+size > allocEndPtr?
	if (rear + size > allocEndPtr)
	{
		wprintf_s(L"Enqueue() Error:rear+size > allocEndPtr\n");
		return;
	}

	//rear ������ �����͸� �ְ�
	//rear++
	//iSize��ŭ �ݺ�
	for (int i = 0; i < size; i++)
	{
		*rear = *value++;
		rear++;
	}
}

void CPacket::Dequeue(char* value, int size)
{
	//front + size > rear?
	if (front + size > rear)
	{
		wprintf_s(L"Dequeue() Error:front+size > rear\n");
		return;
	}

	//�����Ϳ� front���� �ְ�
	//front++
	//iSize��ŭ �ݺ�
	for (int i = 0; i < size; i++)
	{
		*value++ = *front;
		front++;
	}
}

int	CPacket::GetData(char* chpDest, int iSize)
{
	Dequeue(chpDest, iSize);
	return iSize;
}

int	CPacket::PutData(char* chpSrc, int iSrcSize)
{
	Enqueue(chpSrc, iSrcSize);
	return iSrcSize;
}

void CPacket::UpdateHeaderSize(int packetSize) 
{
	// ��Ŷ ũ�� ������ ���� 2��° ����Ʈ ��ġ
	//*(GetBufferPtr() + 1) = static_cast<char>(packetSize);

	// PACKET_HEADER ����ü�� ������ ó���� ��ġ�Ѵٰ� ����
	tagPACKET_HEADER* pHeader = reinterpret_cast<tagPACKET_HEADER*>(GetBufferPtr());
	pHeader->BYTEbySize = static_cast<char>(packetSize);
}