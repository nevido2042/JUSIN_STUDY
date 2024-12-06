#include "pch.h"
#include "CScene.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CLineMgr.h"
#include "CKeyMgr.h"

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
}

void CScene::Update()
{

	CObjMgr::Get_Instance()->Update();


}

void CScene::LateUpdate()
{

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




