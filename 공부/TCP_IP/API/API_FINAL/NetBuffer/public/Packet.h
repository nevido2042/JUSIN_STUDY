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
    연산자 오버로딩
    ============================================================================= */

    // 데이터 삽입 (Enqueue)
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

    // 데이터 추출 (Dequeue)
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
    데이터 복사
    ============================================================================= */
    int GetData(_byte* pDest, int iSize);
    int PutData(_byte* pSrc, int iSize);

    void UpdateHeaderSize(int iSize);

protected:
    // 내부 처리 함수 (Enqueue / Dequeue)
    void Enqueue(_byte* pData, int iSize);
    void Dequeue(_byte* pBuf, int iSize);

private:
    // 할당 위치
    _byte* m_pAlloc = nullptr;
    _byte* m_pAllocEnd = nullptr;

    _byte* m_Front = nullptr;
    _byte* m_Rear = nullptr;

    int m_iBufferSize = 0;
    int m_iDataSize = 0; // 현재 버퍼에 사용 중인 사이즈
};
