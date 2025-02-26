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

	//���� ��� ���� �뷮 ���
	int GetUseSize();

	//���� ���ۿ� ���� �뷮 ���
	int GetFreeSize();

	//������ ����
	int Enqueue(_byte* chpData, int iSize);

	//������ ��
	int Dequeue(_byte* chpDest, int iSize);

	//������ ����
	int Peek(_byte* chpDest, int iSize);

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
	_byte* GetFrontBufferPtr();
	_byte* GetRearBufferPtr();
private:
	//���� �Ҵ� ���� ����
	_byte* m_pBufferAlloc;
	//���� �Ҵ� �� ����
	_byte* m_pBufferAllocEnd;
	//���� ������
	int m_iBufferSize;
	//m_front
	_byte* m_front;
	//m_rear
	_byte* m_rear;

	//���� ��ġ ��ȯ
	_byte* NextPos(_byte** pchPos);
	//���� ��ġ ��ȯ
	_byte* PrevPos(_byte** pchPos);
};