#define DEFAULT_BUF_SIZE 100

class CRingBuffer
{
public:
	//100byte
	CRingBuffer();

	CRingBuffer(int iBufferSize);

	void FreeRingBuffer();

	int GetBufferSize();

	//���� ��� ���� �뷮 ���
	int GetUseSize();

	//���� ���ۿ� ���� �뷮 ���
	int GetFreeSize();

	//������ ����
	int Enqueue(char* chpData, int iSize);

	//������ ��
	int Dequeue(char* chpDest, int iSize);

	//������ ����
	int Peek(char* chpDest, int iSize);

	//���� ������ ����
	void ClearBuffer();

	//[][f][][r]*[][][][][]* *~* ������(�ܺο��� �ѹ濡 �� �� �ִ� ������)
	//[][r]*[][]*[f][][][][] *~* ������(�ܺο��� �ѹ濡 �� �� �ִ� ������)
	int DirectEnqueueSize();

	//[][r][][f]*[][][][][]* *~* ������(�ܺο��� �ѹ濡 ���� �� �ִ� ������)
	//[][f]*[][][r]*[][][][] *~* ������(�ܺο��� �ѹ濡 ���� �� �ִ� ������)
	int DirectDequeueSize();

	//���ϴ� ��ŭ �����(���� ��ġ �̵�)
	int MoveRear(int iSize);

	//���ϴ� ��ŭ ����
	int MoveFront(int iSize);

	//m_front m_rear ������ ����
	char* GetFrontBufferPtr();
	char* GetRearBufferPtr();
private:
	//���� �Ҵ� ���� ����
	char* m_pBufferAlloc;
	//���� �Ҵ� �� ����
	char* m_pBufferAllocEnd;
	//���� ������
	int m_iBufferSize;
	//m_front
	char* m_front;
	//m_rear
	char* m_rear;

	//���� ��ġ ��ȯ
	char* NextPos(char** pchPos);
	//���� ��ġ ��ȯ
	char* PrevPos(char** pchPos);
};