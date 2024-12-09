#pragma once

#include "CScene.h"
#include "Define.h"

class CScene_Start :  public  CScene
{

public :
	CScene_Start();
	virtual~CScene_Start();
public :

	virtual void Update() ;
	virtual void Late_Update();
	virtual void Render(HDC _hDC);
	virtual void Release() ;

	// CScene을(를) 통해 상속됨
	void Initialize() override;

	void LateUpdate() override;
private:
	bool		m_bSelectScene;
	RECT		m_tSelectRect;
	unsigned int		m_iRectPos;

	void Update_RectPos();
	void Select_Scene();
};

