#pragma once
#include <Windows.h>
#include <type_traits> // static_assert 사용을 위해 필요
#include "Define.h"

class CPacket
{
public:
    /*---------------------------------------------------------------
    Packet Enum
    ---------------------------------------------------------------*/
    enum en_PACKET
    {
        eBUFFER_DEFAULT = 1400 // 패킷의 기본 버퍼 사이즈
    };

    // 생성자 / 소멸자
    CPacket();
    CPacket(int iSize);
    virtual ~CPacket();

    // 초기화 및 상태 확인
    void Clear(void);
    int GetBufferSize(void);
    int GetDataSize(void);
    _byte* GetBufferPtr(void);

    // 버퍼 포지션 이동
    int MoveWritePos(int iSize);
    int MoveReadPos(int iSize);

    /* =============================================================================
    연산자 오버로딩 (템플릿)
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
    데이터 복사
    ============================================================================= */
    int GetData(_byte* pDest, int iSize);
    int PutData(_byte* pSrc, int iSize);

    void UpdateHeaderSize(int iSize);

protected:
    // 내부 처리 함수 (Enqueue / Dequeue)
    void Enqueue(const _byte* pData, int iSize) 
    {
        if (iSize <= 0 || !pData)
            return;

        if (m_iDataSize + iSize > m_iBufferSize) // 버퍼 오버플로우 방지
            return;

        memcpy(m_Rear, pData, iSize);
        m_Rear += iSize;
        m_iDataSize += iSize;
    }

    void Dequeue(_byte* pBuf, int iSize)
    {
        if (iSize <= 0 || !pBuf)
            return;

        if (m_iDataSize < iSize) // 데이터 부족 방지
            return;

        memcpy(pBuf, m_Front, iSize);
        m_Front += iSize;
        m_iDataSize -= iSize;
    }

private:
    // 할당 위치
    _byte* m_pAlloc = nullptr;
    _byte* m_pAllocEnd = nullptr;

    _byte* m_Front = nullptr;
    _byte* m_Rear = nullptr;

    int m_iBufferSize = 0;
    int m_iDataSize = 0; // 현재 버퍼에 사용 중인 사이즈
};
