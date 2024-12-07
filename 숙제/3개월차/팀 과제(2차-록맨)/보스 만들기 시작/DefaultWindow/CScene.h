#pragma once

#include "Define.h"
#include "CPlayer.h"
#include "CMonster.h"


class CScene
{
public:
	CScene();
	virtual ~CScene();


public:


	virtual void Initialize();  // �����Ҵ���ʱ�ȭ

	virtual void Update(); // �ǽð� ����

	virtual void LateUpdate();

	virtual void Release(); // �����Ҵ�����

	virtual void Render(HDC _hDC);

	virtual void Exit()PURE;// �ش� ������ ������ ȣ��



protected:
	//������Ʈ�� ���� ������ ����
	  
	vector<CObj*> m_arrObj[(UINT)OBJID::OBJ_END]; //����� ��Ʈ ����ȯ / ������Ʈ�� ���� �� ������ ���͸� �׷� ������ŭ ����!


public:


protected:

	void AddObject(CObj* _pObj, GROUP_TYPE _eType)
	{
		m_arrObj[(UINT)_eType].push_back(_pObj);
	}


};

