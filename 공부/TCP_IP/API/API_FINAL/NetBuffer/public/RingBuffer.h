#define DEFAULT_BUF_SIZE 1024
typedef char _byte;
class CRingBuffer
{
public:
	//100byte
	CRingBuffer();

	CRingBuffer(int iBufferSize);

	void FreeRingBuffer();

	int GetBufferSize();

	//현재 사용 중인 용량 얻기
	int GetUseSize();

	//현재 버퍼에 남은 용량 얻기
	int GetFreeSize();

	//데이터 넣음
	int Enqueue(_byte* chpData, int iSize);

	//데이터 뺌
	int Dequeue(_byte* chpDest, int iSize);

	//데이터 보기
	int Peek(_byte* chpDest, int iSize);

	//버퍼 데이터 삭제
	void ClearBuffer();

	//[][f][][r]*[][][][][]* *~* 사이즈(외부에서 한방에 쓸 수 있는 사이즈)
	//[][r]*[][]*[f][][][][] *~* 사이즈(외부에서 한방에 쓸 수 있는 사이즈)
	int DirectEnqueueSize();

	//[][r][][f]*[][][][][]* *~* 사이즈(외부에서 한방에 읽을 수 있는 사이즈)
	//[][f]*[][][r]*[][][][] *~* 사이즈(외부에서 한방에 읽을 수 있는 사이즈)
	int DirectDequeueSize();

	//원하는 만큼 덮어쓰기(쓰기 위치 이동)
	int MoveRear(int iSize);

	//원하는 만큼 삭제
	int MoveFront(int iSize);

	//m_front m_rear 포인터 얻음
	_byte* GetFrontBufferPtr();
	_byte* GetRearBufferPtr();
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

	//다음 위치 반환
	_byte* NextPos(_byte** pchPos);
	//이전 위치 반환
	_byte* PrevPos(_byte** pchPos);
};