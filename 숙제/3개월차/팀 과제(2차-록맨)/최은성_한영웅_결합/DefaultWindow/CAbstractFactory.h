#pragma once

#include "CObj.h"
#include "CAnimation.h"
#include "CBmpMgr.h"
#include "CRolling_Cutter.h"

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
	static CObj* Create(const float& _fX, const float& _fY, const DIRECTION& _eDir, CObj* _Caster, CObj* _pTarget)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(_fX, _fY);
		pObj->Set_Direction(_eDir);
		static_cast<CRolling_Cutter*>(pObj)->Set_Caster(_Caster);
		pObj->Set_Target(_pTarget);
		pObj->Initialize();
		return pObj;
	}

	static CAnimation* Create(CObj* _Target, FPOINT _tSize, int _iDuration, int _iMaxFrame, const TCHAR* _ImgKey)
	{
		CAnimation* pAnim = new T;
		pAnim->Initialize();
		pAnim->Set_hMemDC(CBmpMgr::Get_Instance()->Find_Image(_ImgKey));
		pAnim->Set_Target(_Target);
		pAnim->Set_Vector(ANINFO({ 0.f,0.f }
			, { _Target->Get_Info().fX,_Target->Get_Info().fY }
			, _tSize
			, _Target->Get_Info()
			, _iDuration
			, _iMaxFrame
		));

		return pAnim;
	}

	static CObj* Create(const float& _fX, const float& _fY, CObj* _pTarget)
	{
		CObj* pObj = new T;
		pObj->Set_Pos(_fX, _fY);
		pObj->Set_Target(_pTarget);
		pObj->Initialize();
		return pObj;
	}
};


