
#define DEFAULT_BUF_SIZE 1024
//#include "RingBuffer.h"

#include "../../../EngineSDK/Inc/Engine_Defines.h"
#include "../public/RingBuffer.h"

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

_byte* CRingBuffer::Set_Next(_byte** ppPos)
{
    if (*ppPos == m_pBufferAllocEnd)
        *ppPos = m_pBufferAlloc;
    else
        *ppPos += 1;

    return *ppPos;
}

_byte* CRingBuffer::Set_Prev(_byte** ppPos)
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

CRingBuffer::~CRingBuffer()
{
    Release();
}

int CRingBuffer::Get_BufferSize()
{
    return m_iBufferSize;
}

int CRingBuffer::Get_UseSize()
{
    if (m_front <= m_rear)
    {
        return static_cast<int>(m_rear - m_front);
    }
    return static_cast<int>(m_pBufferAllocEnd - m_front) + static_cast<int>(m_rear - m_pBufferAlloc);
}

int CRingBuffer::Get_FreeSize()
{
    return m_iBufferSize - Get_UseSize();
}

int CRingBuffer::Enqueue(_byte* pData, int iSize)
{
    if (Get_FreeSize() < iSize)
    {
        wprintf_s(L"Enqueue() error: �������� �������� ũ�Ⱑ ���� ����� �ʰ�\n");
        return 0;
    }
    for (int i = 0; i < iSize; i++)
    {
        *Set_Next(&m_rear) = pData[i];
    }
    return iSize;
}

int CRingBuffer::Dequeue(_byte* pDest, int iSize)
{
    if (Get_UseSize() < iSize)
    {
        wprintf_s(L"Dequeue() error: ������ ũ�⺸�� ���� ���� ����.\n");
        return 0;
    }
    for (int i = 0; i < iSize; i++)
    {
        pDest[i] = *Set_Next(&m_front);
    }
    return iSize;
}

int CRingBuffer::Peek(_byte* pDest, int iSize)
{
    if (Get_UseSize() < iSize)
    {
        wprintf_s(L"Peek() error: Ȯ���Ϸ��� ũ�Ⱑ �����ִ� �����ͺ��� ����.\n");
        return 0;
    }
    _byte* pFrontCpy = m_front;
    for (int i = 0; i < iSize; i++)
    {
        pDest[i] = *Set_Next(&pFrontCpy);
    }
    return iSize;
}

void CRingBuffer::Clear()
{
    m_front = m_rear;
}

int CRingBuffer::DirectEnqueueSize()
{
    if (m_rear < m_front)
    {
        return static_cast<int>((m_front - 1) - m_rear);
    }
    else
    {
        return static_cast<int>(m_pBufferAllocEnd - m_rear);
    }
}

int CRingBuffer::DirectDequeueSize()
{
    if (m_rear < m_front)
    {
        return static_cast<int>(m_pBufferAllocEnd - m_front);
    }
    else
    {
        return static_cast<int>(m_rear - m_front);
    }
}

int CRingBuffer::Move_Rear(int iSize)
{
    //���� ���� ���� ũ�� �ȵ�
    if (Get_FreeSize() < iSize)
    {
        wprintf_s(L"Move_Rear() error:���� ũ�⺸�� �������� ũ�Ⱑ Ů�ϴ�.\n");
        return 0;
    }

    int iMoveSize = 0;
    for (int i = 0; i < iSize; i++)
    {
        Set_Next(&m_rear);
        iMoveSize++;
    }
    return iMoveSize;
}

int CRingBuffer::Move_Front(int iSize)
{
    //����ϰ� �ִ� �ͺ��� ũ�� �ȵ�
    if (Get_UseSize() < iSize)
    {
        wprintf_s(L"Move_Front() error:����ϰ� �ִ� ũ�⺸�� �����Ϸ��� ũ�Ⱑ Ů�ϴ�.\n");
        return 0;
    }

    int iMoveSize = 0;
    for (int i = 0; i < iSize; i++)
    {
        Set_Next(&m_front);
        iMoveSize++;
    }
    return iMoveSize;
}

_byte* CRingBuffer::Get_Front()
{
    return m_front;
}

_byte* CRingBuffer::Get_Rear()
{
    return m_rear;
}
