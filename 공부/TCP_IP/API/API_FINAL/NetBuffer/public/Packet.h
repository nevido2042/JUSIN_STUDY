#pragma once
#include <Windows.h>
#include <type_traits> // static_assert ����� ���� �ʿ�
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
    ������ �����ε� (���ø�)
    ============================================================================= */
    template <typename T>
    CPacket& operator<<(const T& value)
    {
        static_assert(std::is_trivially_copyable<T>::value, "T must be trivially copyable");
        Enqueue(reinterpret_cast<const _byte*>(&value), sizeof(T));
        return *this;
    }

    template <typename T>
    CPacket& operator>>(T& value)
    {
        static_assert(std::is_trivially_copyable<T>::value, "T must be trivially copyable");
        Dequeue(reinterpret_cast<_byte*>(&value), sizeof(T));
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
    void Enqueue(const _byte* pData, int iSize) 
    {
        if (iSize <= 0 || !pData)
            return;

        if (m_iDataSize + iSize > m_iBufferSize) // ���� �����÷ο� ����
            return;

        memcpy(m_Rear, pData, iSize);
        m_Rear += iSize;
        m_iDataSize += iSize;
    }

    void Dequeue(_byte* pBuf, int iSize)
    {
        if (iSize <= 0 || !pBuf)
            return;

        if (m_iDataSize < iSize) // ������ ���� ����
            return;

        memcpy(pBuf, m_Front, iSize);
        m_Front += iSize;
        m_iDataSize -= iSize;
    }

private:
    // �Ҵ� ��ġ
    _byte* m_pAlloc = nullptr;
    _byte* m_pAllocEnd = nullptr;

    _byte* m_Front = nullptr;
    _byte* m_Rear = nullptr;

    int m_iBufferSize = 0;
    int m_iDataSize = 0; // ���� ���ۿ� ��� ���� ������
};
