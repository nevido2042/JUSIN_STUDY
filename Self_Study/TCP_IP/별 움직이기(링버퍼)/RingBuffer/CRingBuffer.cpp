#include "CRingBuffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

char* CRingBuffer::NextPos(char** pchPos)
{
    if (*pchPos == m_pBufferAllocEnd)
        *pchPos = m_pBufferAlloc;
    else
        (*pchPos)++;

    return *pchPos;
}

char* CRingBuffer::PrevPos(char** pchPos)
{
    if (*pchPos == m_pBufferAlloc)
        *pchPos = m_pBufferAllocEnd;
    else
        (*pchPos)--;

    return *pchPos;
}

CRingBuffer::CRingBuffer()
{
    m_pBufferAlloc = (char*)malloc(DEFAULT_BUF_SIZE + 1);
    if (m_pBufferAlloc == nullptr)
    {
        wprintf_s(L"malloc() error: 버퍼 할당 실패\n");
        exit(1);
    }

    m_iBufferSize = DEFAULT_BUF_SIZE;
    m_pBufferAllocEnd = m_pBufferAlloc + DEFAULT_BUF_SIZE;
    m_front = m_pBufferAlloc;
    m_rear = m_pBufferAlloc;
}

CRingBuffer::CRingBuffer(int iBufferSize)
{
    m_pBufferAlloc = (char*)malloc(iBufferSize + 1);
    if (m_pBufferAlloc == nullptr)
    {
        wprintf_s(L"malloc() error: 버퍼 할당 실패\n");
        exit(1);
    }

    m_iBufferSize = iBufferSize;
    m_pBufferAllocEnd = m_pBufferAlloc + iBufferSize;
    m_front = m_pBufferAlloc;
    m_rear = m_pBufferAlloc;
}

int CRingBuffer::GetBufferSize()
{
    return m_iBufferSize;
}

int CRingBuffer::GetUseSize()
{
    if (m_front <= m_rear)
        return m_rear - m_front;
    else
        return (m_pBufferAllocEnd - m_front) + (m_rear - m_pBufferAlloc);
}

int CRingBuffer::GetFreeSize()
{
    return m_iBufferSize - GetUseSize();
}

int CRingBuffer::Enqueue(char* chpData, int iSize)
{
    if (GetFreeSize() < iSize)
    {
        wprintf_s(L"Enqueue() error: 넣으려는 데이터의 크기가 남은 공간을 초과함\n");
        return 0;
    }

    int iEnqueueSize = 0;
    for (int i = 0; i < iSize; i++)
    {
        *m_rear = chpData[i];
        NextPos(&m_rear);
        iEnqueueSize++;
    }
    return iEnqueueSize;
}

int CRingBuffer::Dequeue(char* chpDest, int iSize)
{
    if (GetUseSize() < iSize)
    {
        wprintf_s(L"Dequeue() error: 빼려는 크기보다 남아 있는 데이터가 적음\n");
        return 0;
    }

    for (int i = 0; i < iSize; i++)
    {
        chpDest[i] = *m_front;
        NextPos(&m_front);
    }
    return iSize;
}

int CRingBuffer::Peek(char* chpDest, int iSize)
{
    if (GetUseSize() < iSize)
    {
        wprintf_s(L"Peek() error: 확인하려는 크기가 남아 있는 데이터보다 많음\n");
        return 0;
    }

    char* frontCpy = m_front;
    for (int i = 0; i < iSize; i++)
    {
        chpDest[i] = *frontCpy;
        NextPos(&frontCpy);
    }
    return iSize;
}

void CRingBuffer::ClearBuffer()
{
    m_front = m_rear;
}

int CRingBuffer::DirectEnqueueSize()
{
    if (m_rear < m_front)
        return (m_front - m_rear - 1);
    else
        return (m_pBufferAllocEnd - m_rear) + (m_front != m_pBufferAlloc ? 1 : 0);
}

int CRingBuffer::DirectDequeueSize()
{
    if (m_front <= m_rear)
        return m_rear - m_front;
    else
        return (m_pBufferAllocEnd - m_front) + (m_rear - m_pBufferAlloc);
}

int CRingBuffer::MoveRear(int iSize)
{
    if (GetFreeSize() < iSize)
    {
        wprintf_s(L"MoveRear() error: 남은 공간보다 이동 크기가 큽니다.\n");
        return 0;
    }

    for (int i = 0; i < iSize; i++)
        NextPos(&m_rear);

    return iSize;
}

int CRingBuffer::MoveFront(int iSize)
{
    if (GetUseSize() < iSize)
    {
        wprintf_s(L"MoveFront() error: 사용하고 있는 크기보다 삭제하려는 크기가 큽니다.\n");
        return 0;
    }

    for (int i = 0; i < iSize; i++)
        NextPos(&m_front);

    return iSize;
}

char* CRingBuffer::GetFrontBufferPtr()
{
    return m_front;
}

char* CRingBuffer::GetRearBufferPtr()
{
    return m_rear;
}
