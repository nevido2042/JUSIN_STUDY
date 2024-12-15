#pragma once

#include "Obj.h"

class CTile :   public CObj
{
public:
	CTile();
	virtual ~CTile();

public:
	const TILEOPT	Get_Option() { return m_eOption; }
	CObj*		Get_Obj() { return m_pObj; }

	void			Set_DrawID(int _iDrawID) { m_iDrawID = _iDrawID;  }
	void			Set_Option(TILEOPT _eOption) { m_eOption = _eOption; }
	void			Set_Obj(CObj* _pObj) { m_pObj = _pObj; }
	

public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

private:
	TILEOPT		m_eOption;
	CObj*		m_pObj;
};

