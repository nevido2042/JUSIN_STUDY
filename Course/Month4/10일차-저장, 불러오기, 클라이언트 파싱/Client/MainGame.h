#pragma once

#include "Include.h"
#include "CTextureMgr.h"

class CDevice;
class CMainGame
{
public:
	HRESULT		Initialize(void);
	void		Update(void);
	void		Late_Update(void);
	void		Render(void);
	void		Release(void);

private:
	CDevice*			m_pGraphicDev = nullptr;

public:
	CMainGame();
	~CMainGame();
};

// �÷��̾� �ִϸ��̼� ����
// ���콺 Ŭ���� ��ġ�� ������ �̵�
// ���� �켱 Ž���� �ʺ� �켱 Ž���� �������� �˾ƿͶ�