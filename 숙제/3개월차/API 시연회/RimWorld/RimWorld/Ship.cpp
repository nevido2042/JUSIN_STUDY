#include "pch.h"
#include "Ship.h"
#include "ObjMgr.h"
#include "Pawn.h"

CShip::CShip()
{
}

CShip::~CShip()
{
	Release();
}

void CShip::Initialize()
{
	Set_ImgKey(L"Wall_Atlas_Smooth");�̰� �̹��� �ٲ����

	m_tInfo.fCX = 128.f;
	m_tInfo.fCY = 128.f;

	m_eRenderID = RENDER_GAMEOBJECT;

	//�۾� ��������� ������ �˸���.
	CColonyMgr::Get_Instance()->Notify_TaskChange();

	//���鿡�� ���� �ٲ���� ���� ������ �˸���, �׺���̼� ����
	for (CObj* pObj : CObjMgr::Get_Instance()->Get_List()[OBJ_ENEMY])
	{
		//static_cast<CPawn*>(pObj)->RequestNavStop();
		static_cast<CPawn*>(pObj)->Set_Navigating(false);//�ƴѰ� ���⵵�ϰ�
		static_cast<CPawn*>(pObj)->Set_Navigating(true);
	}

	for (CObj* pObj : CObjMgr::Get_Instance()->Get_List()[OBJ_RIM])
	{
		static_cast<CPawn*>(pObj)->Set_Navigating(false);//�ƴѰ� ���⵵�ϰ�
		static_cast<CPawn*>(pObj)->Set_Navigating(true);
	}
}

int CShip::Update()
{
	if (m_bDestroyed)
	{
		return OBJ_DESTROYED;
	}

	__super::Update_Rect();

	return OBJ_NOEVENT;
}

void CShip::Late_Update()
{
}

void CShip::Render(HDC hDC)
{
}

void CShip::Release()
{
}
