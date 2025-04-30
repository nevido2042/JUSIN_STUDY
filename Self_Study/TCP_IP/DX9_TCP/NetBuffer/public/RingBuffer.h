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
	int Enqueue(_BYTE* pData, int iSize);
	//������ ��
	int Dequeue(_BYTE* pDest, int iSize);
	//������ ����
	int Peek(_BYTE* pDest, int iSize);
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
	_BYTE* Get_Front();
	_BYTE* Get_Rear();
private:
	//���� ��ġ ��ȯ
	_BYTE* Set_Next(_BYTE** ppPos);
	//���� ��ġ ��ȯ
	_BYTE* Set_Prev(_BYTE** ppPos);
private:
	//���� �Ҵ� ���� ����
	_BYTE* m_pBufferAlloc;
	//���� �Ҵ� �� ����
	_BYTE* m_pBufferAllocEnd;
	//���� ������
	int m_iBufferSize;
	//m_front
	_BYTE* m_front;
	//m_rear
	_BYTE* m_rear;
};