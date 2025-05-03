
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
        free(m_pBufferAlloc);  // 메모리 해제
        m_pBufferAlloc = nullptr;  // 포인터 초기화 (안전성을 위해)
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
        wprintf_s(L"malloc() error: 버퍼 할당 실패\n");
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
        wprintf_s(L"Enqueue() error: 넣으려는 데이터의 크기가 남은 사이즈를 초과\n");
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
        wprintf_s(L"Dequeue() error: 빼려는 크기보다 남은 것이 적다.\n");
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
        wprintf_s(L"Peek() error: 확인하려는 크기가 남아있는 데이터보다 많다.\n");
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
    //여유 공간 보다 크면 안됨
    if (Get_FreeSize() < iSize)
    {
        wprintf_s(L"Move_Rear() error:여유 크기보다 덮으려는 크기가 큽니다.\n");
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
    //사용하고 있는 것보다 크면 안됨
    if (Get_UseSize() < iSize)
    {
        wprintf_s(L"Move_Front() error:사용하고 있는 크기보다 삭제하려는 크기가 큽니다.\n");
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
