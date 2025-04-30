#include "RingBuffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

void CRingBuffer::Release()
{
    if (m_pBufferAlloc != nullptr)
    {
        free(m_pBufferAlloc);  // �޸� ����
        m_pBufferAlloc = nullptr;  // ������ �ʱ�ȭ (�������� ����)
    }
}

void CRingBuffer::FreeRingBuffer()
{
    free(m_pBufferAlloc);
}

_byte* CRingBuffer::NextPos(_byte** ppPos)
{
    if (*ppPos == m_pBufferAllocEnd)
        *ppPos = m_pBufferAlloc;
    else
        *ppPos += 1;

    return *ppPos;
}

_byte* CRingBuffer::PrevPos(_byte** ppPos)
{
    if (*ppPos == m_pBufferAlloc)
        *ppPos = m_pBufferAllocEnd;
    else
        *ppPos -= 1;

    return *ppPos;
}


CRingBuffer::CRingBuffer()
{
    m_pBufferAlloc = (_byte*)malloc(DEFAULT_BUF_SIZE + 1);
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

//CRingBuffer::CRingBuffer(int iBufferSize)
//{
//    m_pBufferAlloc = (_byte*)malloc(iBufferSize + 1);
//    if (m_pBufferAlloc == nullptr)
//    {
//        wprintf_s(L"malloc() error: ���� �Ҵ� ����\n");
//        exit(EXIT_FAILURE);
//    }
//    m_iBufferSize = iBufferSize;
//    m_pBufferAllocEnd = m_pBufferAlloc + iBufferSize;
//    m_front = m_pBufferAlloc;
//    m_rear = m_front;
//}

CRingBuffer::~CRingBuffer()
{
    Release();
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

int CRingBuffer::Enqueue(_byte* pData, int iSize)
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

int CRingBuffer::Dequeue(_byte* pDest, int iSize)
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

int CRingBuffer::Peek(_byte* pDest, int iSize)
{
    if (GetUseSize() < iSize)
    {
        wprintf_s(L"Peek() error: Ȯ���Ϸ��� ũ�Ⱑ �����ִ� �����ͺ��� ����.\n");
        return 0;
    }
    _byte* pFrontCpy = m_front;
    for (int i = 0; i < iSize; i++)
    {
        pDest[i] = *NextPos(&pFrontCpy);
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

_byte* CRingBuffer::GetFrontBufferPtr()
{
    return m_front;
}

_byte* CRingBuffer::GetRearBufferPtr()
{
    return m_rear;
}
