#pragma once
#include "Obj.h"

typedef struct tagTask
{
	enum TYPE { WALL, SHIP, DECONSTRUCT };

	tagTask()
		:pObj(nullptr), pRimReserved(nullptr) {}

	CObj* pObj; //�۾��� ��... �Ǽ� �ϱ����� Ÿ�� / ��ü�ϱ� ���� ��
	CObj* pRimReserved;//�� �۾��� ���� �����ߴ���
	TYPE  eType;

	//������ �Ǽ��ϴ°�?
	

	// pObj���� �������� �� ������ ����
	bool operator<(const tagTask& other) const
	{
		return pObj < other.pObj;  // pObj�� �������� ��
	}
}TASK;

class CColonyMgr
{
public:
	enum MODE { MODE_SELECT, MODE_DECONSTRUCT, MODE_CONSTRUCT, MODE_SHIP, MODE_END };
private:
	CColonyMgr();
	virtual ~CColonyMgr();
public:
	void					Change_Mode(MODE _eMode);
	void					Emplace_DeconstructSet(TASK _tTask);
	void					Emplace_ConstructSet(TASK _tTask);
	//void					Emplace_TransportSet(TASK _tTask);
	void					Notify_TaskChange();
public:
	CObj*					Get_Target() { return m_pTarget; }
	void					Set_Target(CObj* _pObj);
	MODE					Get_Mode() { return m_eMode; }

	set<TASK>*				Get_DeconstructSet()
	{
		return &m_DeconstructSet; 
	}
	set<TASK>*				Get_ConstructSet() 
	{
		return &m_ConstructSet; 
	}
	/*set<TASK>*				Get_TransportSet() 
	{ 
		return &m_TransportSet; 
	}*/

private:
	void	Input_Key();

public:
	void	Initialize();
	int		Update();
	void	Late_Update();
	void	Render(HDC hDC);
	void	Release();
private:
	void	Control_Target();

public:
	static CColonyMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CColonyMgr;

		return m_pInstance;
	}

	static void	Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CColonyMgr* m_pInstance;

private:
	CObj*	m_pTarget;
	MODE	m_eMode;

	//��ü ����Ʈ //���� ����? ��?
	//�� �����̳ʷ� ���� �ּҰ��� �������Ҷ� ��������?
	//���⼭ ������ ����Ʈ���ֳ�?
	set<TASK>	m_DeconstructSet;
	set<TASK>	m_ConstructSet;
	//set<TASK>	m_TransportSet;

};

