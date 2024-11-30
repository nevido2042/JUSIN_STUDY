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
protected:
	void SpawnMonster(); //���࿡ �������� ���еǸ� virtual��
	void SpawnItem(float _x, float _y); 
private:
	ULONGLONG	m_dwTime;
	bool m_bFinish;
};

