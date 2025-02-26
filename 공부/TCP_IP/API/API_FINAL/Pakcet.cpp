#include"Pakcet.h"
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
	front = rear;
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
	//rear을 넘어가면 안된다.
	if (front + iSize > rear)
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

	front += iSize;
	return iSize;
}

int CPacket::MoveWritePos(int iSize)
{
	//rear가 할당 끝지점을 넘어서면 안된다.
	if (rear + iSize > allocEndPtr)
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

	//rear 지점에 데이터를 넣고
	//rear++
	//iSize만큼 반복
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

	//데이터에 front값을 넣고
	//front++
	//iSize만큼 반복
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