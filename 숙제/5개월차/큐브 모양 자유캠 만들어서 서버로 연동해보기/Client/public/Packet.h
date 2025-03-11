#pragma once
//#include <Windows.h>
#include "Network_Define.h"

class CPacket
{
public:
    /* =============================================================================
    연산자 오버로딩 (템플릿)
    ============================================================================= */

    // 데이터 삽입 (Enqueue)
    template <typename T>
    CPacket& operator << (const T& value)
    {
        Enqueue(reinterpret_cast<const _BYTE*>(&value), sizeof(T));
        return *this;
    }

    // 데이터 추출 (Dequeue)
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
        eBUFFER_DEFAULT = 1400 // 패킷의 기본 버퍼 사이즈
    };

public:
    // 생성자 / 소멸자
    CPacket();
    CPacket(int iSize);
    virtual ~CPacket();

public:
    // 초기화 및 상태 확인
    void Clear(void);
    int Get_BufferSize(void);
    int Get_DataSize(void);
    _BYTE* Get_BufferPtr(void);

    // 버퍼 포지션 이동
    int Move_WritePos(int iSize);
    int Move_ReadPos(int iSize);

    int Get_Data(_BYTE* pDest, int iSize);
    int Put_Data(_BYTE* pSrc, int iSize);

    void Update_HeaderSize(int iSize);

private:
    void Enqueue(const _BYTE* pData, int iSize);
    void Dequeue(_BYTE* pBuf, int iSize);

private:
    // 할당 위치
    _BYTE* m_pAlloc = nullptr;
    _BYTE* m_pAllocEnd = nullptr;

    _BYTE* m_Front = nullptr;
    _BYTE* m_Rear = nullptr;

    int m_iBufferSize = 0;
    int m_iDataSize = 0; // 현재 버퍼에 사용 중인 사이즈
};
