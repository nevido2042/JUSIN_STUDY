#include "CRingBuffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

void CRingBuffer::FreeRingBuffer()
{
    free(m_pBufferAlloc);
}

char* CRingBuffer::NextPos(char** ppPos)
{
    *ppPos = (*ppPos == m_pBufferAllocEnd) ? m_pBufferAlloc : (*ppPos + 1);
    return *ppPos;
}

char* CRingBuffer::PrevPos(char** pchPos)
{
    *pchPos = (*pchPos == m_pBufferAlloc) ? m_pBufferAllocEnd : (*pchPos - 1);
    return *pchPos;
}

CRingBuffer::CRingBuffer()
{
    m_pBufferAlloc = (char*)malloc(DEFAULT_BUF_SIZE + 1);
    if (m_pBufferAlloc == nullptr)
    {
        wprintf_s(L"malloc() error: ���� �Ҵ� ����\n");
        exit(EXIT_FAILURE);
    }
    m_iBufferSize = DEFAULT_BUF_SIZE;
    m_pBufferAllocEnd = m_pBufferAlloc + DEFAULT_BUF_SIZE;
    m_front = m_pBufferAlloc;
    m_rear = m_front;
}

CRingBuffer::CRingBuffer(int iBufferSize)
{
    m_pBufferAlloc = (char*)malloc(iBufferSize + 1);
    if (m_pBufferAlloc == nullptr)
    {
        wprintf_s(L"malloc() error: ���� �Ҵ� ����\n");
        exit(EXIT_FAILURE);
    }
    m_iBufferSize = iBufferSize;
    m_pBufferAllocEnd = m_pBufferAlloc + iBufferSize;
    m_front = m_pBufferAlloc;
    m_rear = m_front;
}

int CRingBuffer::GetBufferSize()
{
    return m_iBufferSize;
}

int CRingBuffer::GetUseSize()
{
    if (m_front <= m_rear)
    {
        return m_rear - m_front;
    }
    return (m_pBufferAllocEnd - m_front) + (m_rear - m_pBufferAlloc);
}

int CRingBuffer::GetFreeSize()
{
    return m_iBufferSize - GetUseSize();
}

int CRingBuffer::Enqueue(char* pData, int iSize)
{
    if (GetFreeSize() < iSize)
    {
        wprintf_s(L"Enqueue() error: �������� �������� ũ�Ⱑ ���� ����� �ʰ�\n");
        return 0;
    }
    for (int i = 0; i < iSize; i++)
    {
        *NextPos(&m_rear) = pData[i];
    }
    return iSize;
}

int CRingBuffer::Dequeue(char* pDest, int iSize)
{
    if (GetUseSize() < iSize)
    {
        wprintf_s(L"Dequeue() error: ������ ũ�⺸�� ���� ���� ����.\n");
        return 0;
    }
    for (int i = 0; i < iSize; i++)
    {
        pDest[i] = *NextPos(&m_front);
    }
    return iSize;
}

int CRingBuffer::Peek(char* chpDest, int iSize)
{
    if (GetUseSize() < iSize)
    {
        wprintf_s(L"Peek() error: Ȯ���Ϸ��� ũ�Ⱑ �����ִ� �����ͺ��� ����.\n");
        return 0;
    }
    char* frontCpy = m_front;
    for (int i = 0; i < iSize; i++)
    {
        chpDest[i] = *NextPos(&frontCpy);
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
    {
        return (m_front - 1) - m_rear;
    }
    else
    {
        return  m_pBufferAllocEnd - m_rear;
    }
}

int CRingBuffer::DirectDequeueSize()
{
    if (m_rear < m_front)
    {
        return m_pBufferAllocEnd - m_front;
    }
    else
    {
        return m_rear - m_front;
    }
}

int CRingBuffer::MoveRear(int iSize)
{
    //���� ���� ���� ũ�� �ȵ�
    if (GetFreeSize() < iSize)
    {
        wprintf_s(L"MoveRear() error:���� ũ�⺸�� �������� ũ�Ⱑ Ů�ϴ�.\n");
        return 0;
    }

    int iMoveSize = 0;
    for (int i = 0; i < iSize; i++)
    {
        NextPos(&m_rear);
        iMoveSize++;
    }
    return iMoveSize;
}

int CRingBuffer::MoveFront(int iSize)
{
    //����ϰ� �ִ� �ͺ��� ũ�� �ȵ�
    if (GetUseSize() < iSize)
    {
        wprintf_s(L"MoveFront() error:����ϰ� �ִ� ũ�⺸�� �����Ϸ��� ũ�Ⱑ Ů�ϴ�.\n");
        return 0;
    }

    int iMoveSize = 0;
    for (int i = 0; i < iSize; i++)
    {
        NextPos(&m_front);
        iMoveSize++;
    }
    return iMoveSize;
}

char* CRingBuffer::GetFrontBufferPtr()
{
    return m_front;
}

char* CRingBuffer::GetRearBufferPtr()
{
    return m_rear;
}
