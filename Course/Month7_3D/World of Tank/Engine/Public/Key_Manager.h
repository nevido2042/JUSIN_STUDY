#pragma once
#include "Base.h"

#define	VK_MAX		0xff

BEGIN(Engine)

class ENGINE_DLL CKey_Manager final : public CBase
{
private:
	CKey_Manager();
	virtual ~CKey_Manager() = default;

public:
	_bool		Key_Pressing(_int _Key);
	_bool		Key_Up(_int _Key);		// 누르고 있다가 뗐을 때
	_bool		Key_Down(_int _Key);		// 눌렀을 때

private:
	_bool						m_bKeyState[VK_MAX] = { FALSE };

public:
	static CKey_Manager* Create();
	virtual void Free() override;
};

END