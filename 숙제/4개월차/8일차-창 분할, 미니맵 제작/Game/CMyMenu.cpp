#include "pch.h"
#include "CMyMenu.h"
#include "AbstractFactory.h"
#include "CObjMgr.h"
#include "CPlayer.h"
#include "Include.h"

void CMyMenu::Initialize()
{
    CObj* pObj = CAbstractFactory<CPlayer>::Create();
    CObjMgr::Get_Instance()->Add_Object(OBJ_PLAYER, pObj);
}

void CMyMenu::Update()
{
    CObjMgr::Get_Instance()->Update();
}

void CMyMenu::Late_Update()
{
    CObjMgr::Get_Instance()->Late_Update();
}

void CMyMenu::Render()
{
    CObjMgr::Get_Instance()->Render();
}

void CMyMenu::Release()
{
    CObjMgr::DestroyInstance();
}
