#pragma	once

#include "Engine_Defines.h"
#define DEFAULT_BUF_SIZE 2048
class CRingBuffer
{
public:
	//100byte
	CRingBuffer();
	~CRingBuffer();
public:
	void Release();
public:
	int Get_BufferSize();
	//현재 사용 중인 용량 얻기
	int Get_UseSize();
	//현재 버퍼에 남은 용량 얻기
	int Get_FreeSize();
	//데이터 넣음
	int Enqueue(_byte* pData, int iSize);
	//데이터 뺌
	int Dequeue(_byte* pDest, int iSize);
	//데이터 보기
	int Peek(_byte* pDest, int iSize);
	//버퍼 데이터 삭제
	void Clear();
	//[][f][][r]*[][][][][]* *~* 사이즈(외부에서 한방에 쓸 수 있는 사이즈)
	//[][r]*[][]*[f][][][][] *~* 사이즈(외부에서 한방에 쓸 수 있는 사이즈)
	int DirectEnqueueSize();
	//[][r][][f]*[][][][][]* *~* 사이즈(외부에서 한방에 읽을 수 있는 사이즈)
	//[][f]*[][][r]*[][][][] *~* 사이즈(외부에서 한방에 읽을 수 있는 사이즈)
	int DirectDequeueSize();
	//원하는 만큼 덮어쓰기(쓰기 위치 이동)
	int Move_Rear(int iSize);
	//원하는 만큼 삭제
	int Move_Front(int iSize);
	//m_front m_rear 포인터 얻음
	_byte* Get_Front();
	_byte* Get_Rear();
private:
	//다음 위치 반환
	_byte* Set_Next(_byte** ppPos);
	//이전 위치 반환
	_byte* Set_Prev(_byte** ppPos);
private:
	//버퍼 할당 시작 지점
	_byte* m_pBufferAlloc;
	//버퍼 할당 끝 지점
	_byte* m_pBufferAllocEnd;
	//버퍼 사이즈
	int m_iBufferSize;
	//m_front
	_byte* m_front;
	//m_rear
	_byte* m_rear;
};