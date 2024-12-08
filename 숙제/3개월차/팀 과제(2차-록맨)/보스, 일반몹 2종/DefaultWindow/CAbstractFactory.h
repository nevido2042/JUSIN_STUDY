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

	static CObj* Create(INFO* _tInfo)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->Set_Pos(_tInfo->fX, _tInfo->fY);
		pObj->Set_Size(_tInfo->fCX, _tInfo->fCY);

		return pObj;
	}
	static CObj* Create(const float& _fX, const float& _fY, const DIRECTION& _eDir)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(_fX, _fY);
		pObj->Set_Direction(_eDir);
		pObj->Initialize();

		return pObj;
	}
};

