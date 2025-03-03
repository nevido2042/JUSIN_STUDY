#pragma	once
#include "Define.h"
#define DEFAULT_BUF_SIZE 1024

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
	//���� ��� ���� �뷮 ���
	int Get_UseSize();
	//���� ���ۿ� ���� �뷮 ���
	int Get_FreeSize();
	//������ ����
	int Enqueue(_byte* pData, int iSize);
	//������ ��
	int Dequeue(_byte* pDest, int iSize);
	//������ ����
	int Peek(_byte* pDest, int iSize);
	//���� ������ ����
	void Clear();
	//[][f][][r]*[][][][][]* *~* ������(�ܺο��� �ѹ濡 �� �� �ִ� ������)
	//[][r]*[][]*[f][][][][] *~* ������(�ܺο��� �ѹ濡 �� �� �ִ� ������)
	int DirectEnqueueSize();
	//[][r][][f]*[][][][][]* *~* ������(�ܺο��� �ѹ濡 ���� �� �ִ� ������)
	//[][f]*[][][r]*[][][][] *~* ������(�ܺο��� �ѹ濡 ���� �� �ִ� ������)
	int DirectDequeueSize();
	//���ϴ� ��ŭ �����(���� ��ġ �̵�)
	int Move_Rear(int iSize);
	//���ϴ� ��ŭ ����
	int Move_Front(int iSize);
	//m_front m_rear ������ ����
	_byte* Get_Front();
	_byte* Get_Rear();
private:
	//���� ��ġ ��ȯ
	_byte* Set_Next(_byte** ppPos);
	//���� ��ġ ��ȯ
	_byte* Set_Prev(_byte** ppPos);
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
};