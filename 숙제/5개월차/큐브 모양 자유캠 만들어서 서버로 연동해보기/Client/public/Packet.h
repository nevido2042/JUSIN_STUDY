#pragma once
//#include <Windows.h>
#include "Network_Define.h"

class CPacket
{
public:
    /* =============================================================================
    ������ �����ε� (���ø�)
    ============================================================================= */

    // ������ ���� (Enqueue)
    template <typename T>
    CPacket& operator << (const T& value)
    {
        Enqueue(reinterpret_cast<const _BYTE*>(&value), sizeof(T));
        return *this;
    }

    // ������ ���� (Dequeue)
    template <typename T>
    CPacket& operator >> (T& value)
    {
        Dequeue(reinterpret_cast<_BYTE*>(&value), sizeof(T));
        return *this;
    }
public:
    /*---------------------------------------------------------------
    Packet Enum
    ---------------------------------------------------------------*/
    enum en_PACKET
    {
        eBUFFER_DEFAULT = 1400 // ��Ŷ�� �⺻ ���� ������
    };

public:
    // ������ / �Ҹ���
    CPacket();
    CPacket(int iSize);
    virtual ~CPacket();

public:
    // �ʱ�ȭ �� ���� Ȯ��
    void Clear(void);
    int Get_BufferSize(void);
    int Get_DataSize(void);
    _BYTE* Get_BufferPtr(void);

    // ���� ������ �̵�
    int Move_WritePos(int iSize);
    int Move_ReadPos(int iSize);

    int Get_Data(_BYTE* pDest, int iSize);
    int Put_Data(_BYTE* pSrc, int iSize);

    void Update_HeaderSize(int iSize);

private:
    void Enqueue(const _BYTE* pData, int iSize);
    void Dequeue(_BYTE* pBuf, int iSize);

private:
    // �Ҵ� ��ġ
    _BYTE* m_pAlloc = nullptr;
    _BYTE* m_pAllocEnd = nullptr;

    _BYTE* m_Front = nullptr;
    _BYTE* m_Rear = nullptr;

    int m_iBufferSize = 0;
    int m_iDataSize = 0; // ���� ���ۿ� ��� ���� ������
};
