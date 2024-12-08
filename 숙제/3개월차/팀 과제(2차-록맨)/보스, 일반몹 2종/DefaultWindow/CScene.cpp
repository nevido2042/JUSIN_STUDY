#include "pch.h"
#include "CScene.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CLineMgr.h"
#include "CKeyMgr.h"
#include "CWall.h"
#include "CScrollMgr.h"

CScene::CScene()
{
}

CScene::~CScene() // 
{
	Release();
}

void CScene::Initialize()
{
	CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, CAbstractFactory<CPlayer>::Create());

	CLineMgr::Get_Instance()->Initialize();
	/*for (int i = 0; i < 6; ++i)
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ_MONSTER, CAbstractFactory<CMonster>::Create(float(i * 100), float(400), 0.f));
		CObjMgr::Get_Instance()->Get_LastMonster()->Set_Target(CObjMgr::Get_Instance()->Get_Player());
	}
	for (int i = 0; i < 10; ++i)
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ_WALL, CAbstractFactory<CWall>::Create(float(50), float(i * 100), 0.f));
	}*/
}

void CScene::Update()
{

	if (CScrollMgr::Get_Instance()->Update())
		return;


	CObjMgr::Get_Instance()->Update();



}

void CScene::LateUpdate()
{
	if (0 != CScrollMgr::Get_Instance()->Get_Dir())
		return;

	CObjMgr::Get_Instance()->Late_Update();
	CKeyMgr::Get_Instance()->Update();

}

void CScene::Render(HDC _hDC)
{
	Rectangle(_hDC, 0, 0, WINCX, WINCY);

	CLineMgr::Get_Instance()->Render(_hDC);
	CObjMgr::Get_Instance()->Render(_hDC);

}

void CScene::Release()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::OBJ_END; ++i)
	{
		for (size_t j = 0; j < m_arrObj[i].size(); ++i)
		{
			delete m_arrObj[i][j];
		}

	}
}




