#pragma once

#include "CObj.h"

template<typename T>
class CAbstractFactory
{
public:
	CAbstractFactory() {}
	~CAbstractFactory() {};

public:
	static CObj* Create()
	{
		CObj* pObj = new T;
		pObj->Initialize();
		return pObj;
	}

	static CObj* Create(float _fX, float _fY, float _fAngle)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_fX, _fY);
		pObj->Set_Angle(_fAngle);

		return pObj;
	}

	static CObj* Create(OBJID _eOBJID, INFO* _tInfo)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_tInfo->fX, _tInfo->fY);
		pObj->Set_Size(_tInfo->fCX, _tInfo->fCY);
		pObj->Set_OBJID(_eOBJID);

		return pObj;
	}
};

