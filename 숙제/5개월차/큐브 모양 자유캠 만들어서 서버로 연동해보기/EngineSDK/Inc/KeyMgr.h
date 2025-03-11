#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CKeyMgr final : public CBase
{

private:
	CKeyMgr();
	virtual ~CKeyMgr() = default;


public:
	bool		Key_Pressing(int _Key);
	bool		Key_Up(int _Key);		// 누르고 있다가 뗐을 때
	bool		Key_Down(int _Key);		// 눌렀을 때

private:
	bool						m_bKeyState[VK_MAX];

public:
	static CKeyMgr* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END