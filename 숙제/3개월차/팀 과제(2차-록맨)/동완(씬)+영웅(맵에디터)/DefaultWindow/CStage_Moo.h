#pragma once

#include "CScene.h"

class CStage_Moo : public CScene
{
public:
	CStage_Moo();
	~CStage_Moo();

	virtual void Initialize()override;  // �����Ҵ���ʱ�ȭ

	virtual void Update()override; // �ǽð� ����

	virtual void LateUpdate()override;

	virtual void Release()override; // �����Ҵ�����

	virtual void Render(HDC _hDC)override;

	virtual void Exit()override;// �ش� ������ ������ ȣ��

private:


};

