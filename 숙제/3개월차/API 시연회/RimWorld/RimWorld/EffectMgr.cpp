#include "pch.h"
#include "EffectMgr.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"
#include "Effect.h"

CEffectMgr* CEffectMgr::m_pInstance = nullptr;

CEffectMgr::CEffectMgr()
{
}

CEffectMgr::~CEffectMgr()
{
}

void CEffectMgr::Create_Effect(POS _tPos, float _CX, float _CY, const TCHAR* _pImgKey, float _fTime)
{
	CObj* pEffect = CAbstractFactory<CEffect>::Create((float)_tPos.iX, (float)_tPos.iY);
	pEffect->Set_Size(_CX, _CY);
	pEffect->Set_ImgKey(_pImgKey);
	static_cast<CEffect*>(pEffect)->Set_LifeTime(_fTime);
	CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pEffect);
}
