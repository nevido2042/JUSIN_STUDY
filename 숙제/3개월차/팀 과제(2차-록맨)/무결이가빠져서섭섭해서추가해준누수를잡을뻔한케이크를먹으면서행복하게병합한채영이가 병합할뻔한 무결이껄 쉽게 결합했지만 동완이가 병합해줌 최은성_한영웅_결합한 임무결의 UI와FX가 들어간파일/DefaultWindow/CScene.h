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


	virtual void Initialize()PURE; // �����Ҵ���ʱ�ȭ
	virtual void Update()PURE;// �ǽð� ����
	virtual void LateUpdate()PURE;
	virtual void Release()PURE; // �����Ҵ�����
	virtual void Render(HDC _hDC)PURE;



protected:
	//������Ʈ�� ���� ������ ����
	  
	vector<CObj*> m_VecObj[(UINT)OBJID::OBJ_END]; //����� ��Ʈ ����ȯ 
	// ������Ʈ�� ���� �� ������ ���͸� �׷� ������ŭ ����!


public:


protected:

	void AddObject(CObj* _pObj, GROUP_TYPE _eType)
	{
		m_VecObj[(UINT)_eType].push_back(_pObj);
	}


};

