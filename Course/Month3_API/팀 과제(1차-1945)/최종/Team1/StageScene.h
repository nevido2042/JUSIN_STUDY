#pragma once
#include "SceneObj.h"
class StageScene : public SceneObj
{
public:
	StageScene();
	virtual ~StageScene();
public:
	virtual void Initialize(Obj* _pPlayer) override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _hDC) override;
	virtual void Release() override; //�÷��̾� ���� release����
	virtual void End_Scene();
protected:
	virtual void SpawnMonster(); //���࿡ �������� ���еǸ� virtual��
	void SpawnItem(float _x, float _y); 
protected:
	ULONGLONG	m_ulTime;
	ULONGLONG	m_ulStartTime;
	bool		m_bFinish;
	bool		m_bStart;
	bool m_IsNext;
};

