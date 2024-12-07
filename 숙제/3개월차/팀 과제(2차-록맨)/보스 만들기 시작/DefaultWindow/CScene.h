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


	virtual void Initialize();  // 동적할당및초기화

	virtual void Update(); // 실시간 정보

	virtual void LateUpdate();

	virtual void Release(); // 동적할당해제

	virtual void Render(HDC _hDC);

	virtual void Exit()PURE;// 해당 씬에서 나갈때 호출



protected:
	//오브젝트를 저장 관리할 벡터
	  
	vector<CObj*> m_arrObj[(UINT)OBJID::OBJ_END]; //언사인 인트 형변환 / 오브젝트를 저장 및 관리할 벡터를 그룹 개수만큼 선언!


public:


protected:

	void AddObject(CObj* _pObj, GROUP_TYPE _eType)
	{
		m_arrObj[(UINT)_eType].push_back(_pObj);
	}


};

