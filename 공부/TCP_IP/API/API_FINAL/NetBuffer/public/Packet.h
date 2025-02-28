#pragma once
#include <Windows.h>
#include "Define.h"

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

    // ������ / �Ҹ���
    CPacket();
    CPacket(int iSize);
    virtual ~CPacket();

    // �ʱ�ȭ �� ���� Ȯ��
    void Clear(void);
    int GetBufferSize(void);
    int GetDataSize(void);
    _byte* GetBufferPtr(void);

    // ���� ������ �̵�
    int Move_WritePos(int iSize);
    int Move_ReadPos(int iSize);

    int GetData(_byte* pDest, int iSize);
    int PutData(_byte* pSrc, int iSize);

    void UpdateHeaderSize(int iSize);

private:
    void Enqueue(_byte* pData, int iSize);
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
