#define DEFAULT_BUF_SIZE 100

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
	int Enqueue(char* chpData, int iSize);

	//데이터 뺌
	int Dequeue(char* chpDest, int iSize);

	//데이터 보기
	int Peek(char* chpDest, int iSize);

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
	char* GetFrontBufferPtr();
	char* GetRearBufferPtr();
private:
	//버퍼 할당 시작 지점
	char* m_pBufferAlloc;
	//버퍼 할당 끝 지점
	char* m_pBufferAllocEnd;
	//버퍼 사이즈
	int m_iBufferSize;
	//m_front
	char* m_front;
	//m_rear
	char* m_rear;

	//다음 위치 반환
	char* NextPos(char** pchPos);
	//이전 위치 반환
	char* PrevPos(char** pchPos);
};