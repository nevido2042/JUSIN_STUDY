#include "pch.h"
#include "RimSlotMgr.h"
#include "ObjMgr.h"
#include "AbstractFactory.h"

CRimSlotMgr::CRimSlotMgr()
{
}

CRimSlotMgr::~CRimSlotMgr()
{
	Release();
}

void CRimSlotMgr::Initialize()
{
	size_t iSize = CObjMgr::Get_Instance()->Get_List()[OBJ_RIM].size();

	m_tInfo.fX = WINCX * 0.5f - (float)(iSize * 32.f);
	m_tInfo.fY = 0;

	//�ʵ忡 �ִ� ���� ������ŭ. �� ������ ���� �ڽ����� �ִ´�.
	//�� �ڽĵ��� ��ġ���� �����ش�.

	int i(0);
	for (CObj* pRim : CObjMgr::Get_Instance()->Get_List()[OBJ_RIM])
	{
		CObj* pObj = CAbstractFactory<CRimSlot>::Create(m_tInfo.fX + i++ * 80.f, m_tInfo.fY + 64.f);
		CObjMgr::Get_Instance()->Add_Object(OBJ_UI, pObj);

		pObj->Set_Target(pRim);
	}

	m_eRenderID = RENDER_UI;
}

int CRimSlotMgr::Update()
{
	return CObj::Update();
}

void CRimSlotMgr::Late_Update()
{
}

void CRimSlotMgr::Render(HDC hDC)
{
}

void CRimSlotMgr::Release()
{
}
