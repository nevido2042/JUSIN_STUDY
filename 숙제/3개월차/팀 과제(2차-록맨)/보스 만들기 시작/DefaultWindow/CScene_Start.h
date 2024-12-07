#pragma once

#include "CScene.h"
#include "Define.h"

class CScene_Start :  public  CScene
{

public :
	CScene_Start();
	virtual~CScene_Start();
public :

	virtual void Initialize(CObj* _pPlayer) ;
	virtual void Update() ;
	virtual void Late_Update();
	virtual void Render(HDC _hDC);
	virtual void Release() ;




	// CScene��(��) ���� ��ӵ�
	void Exit() override;

};

