#pragma once
#include "Define.h"
#include "SceneObj.h"
#include "Obj.h"

class MainGame
{
public:
	MainGame();
	virtual ~MainGame();
public:
	void Initialize();
	int Update();
	void Late_Update();
	void Render(); 
	void Release();
public:
	void Load_Scene() { m_iScene++; }
private:
	HDC m_hDC;
	SceneObj* m_SceneArray[SCENE::NONE];
	Obj* m_pPlayer;
	int m_iScene;
};

