#pragma once

#include "Engine_Defines.h"

#define PACKET_CODE (_byte)0x20

#pragma pack(push, 1)
struct tagPACKET_HEADER
{
    _byte byCode;
    _byte bySize;
    _byte byType;
};
#pragma pack(pop)

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
        Enqueue(reinterpret_cast<const _byte*>(&value), sizeof(T));
        return *this;
    }

    // ������ ���� (Dequeue)
    template <typename T>
    CPacket& operator >> (T& value)
    {
        Dequeue(reinterpret_cast<_byte*>(&value), sizeof(T));
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
    _byte* Get_BufferPtr(void);

    // ���� ������ �̵�
    int Move_WritePos(int iSize);
    int Move_ReadPos(int iSize);

    int Get_Data(_byte* pDest, int iSize);
    int Put_Data(_byte* pSrc, int iSize);

    void Update_HeaderSize(int iSize);

public:
    void Enqueue(const _byte* pData, int iSize);
    void Dequeue(_byte* pBuf, int iSize);

private:
    // �Ҵ� ��ġ
    _byte* m_pAlloc = nullptr;
    _byte* m_pAllocEnd = nullptr;

    _byte* m_Front = nullptr;
    _byte* m_Rear = nullptr;

    int m_iBufferSize = 0;
    int m_iDataSize = 0; // ���� ���ۿ� ��� ���� ������
};
