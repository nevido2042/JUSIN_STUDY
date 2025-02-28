#pragma once
#include <Windows.h>
#include "Define.h"

class CPacket
{
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
    int MoveWritePos(int iSize);
    int MoveReadPos(int iSize);

    /* =============================================================================
    ������ �����ε�
    ============================================================================= */

    // ������ ���� (Enqueue)
    CPacket& operator << (char chValue)
    {
        Enqueue(&chValue, sizeof(chValue));
        return *this;
    }

    CPacket& operator << (short shValue)
    {
        Enqueue((_byte*)&shValue, sizeof(shValue));
        return *this;
    }

    CPacket& operator << (int iValue)
    {
        Enqueue((_byte*)&iValue, sizeof(iValue));
        return *this;
    }

    // ������ ���� (Dequeue)
    CPacket& operator >> (char& chValue)
    {
        Dequeue(&chValue, sizeof(chValue));
        return *this;
    }

    CPacket& operator >> (short& shValue)
    {
        Dequeue((_byte*)&shValue, sizeof(shValue));
        return *this;
    }

    CPacket& operator >> (int& iValue)
    {
        Dequeue((_byte*)&iValue, sizeof(iValue));
        return *this;
    }

    /* =============================================================================
    ������ ����
    ============================================================================= */
    int GetData(_byte* pDest, int iSize);
    int PutData(_byte* pSrc, int iSize);

    void UpdateHeaderSize(int iSize);

protected:
    // ���� ó�� �Լ� (Enqueue / Dequeue)
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
