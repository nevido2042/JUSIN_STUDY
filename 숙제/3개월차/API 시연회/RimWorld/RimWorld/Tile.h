#pragma once

#include "Obj.h"

class CTile :   public CObj
{
public:
	CTile();
	virtual ~CTile();

public:
	const TILEOPT	Get_Option() { return m_eOption; }

	void			Set_DrawID(int _iDrawID) { m_iDrawID = _iDrawID;  }
	void			Set_Option(TILEOPT _eOption) { m_eOption = _eOption; }

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	TILEOPT		m_eOption;
};

