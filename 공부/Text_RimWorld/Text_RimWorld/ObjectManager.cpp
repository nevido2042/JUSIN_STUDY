#include "pch.h"
#include "ObjectManager.h"
#include "Define.h"

CObjectManager::~CObjectManager()
{
	Release();
}

void CObjectManager::Initialize()
{
}

void CObjectManager::Update()
{
	for (CObject* pObject : m_ObjectList)
	{
		pObject->Update();
	}
}

void CObjectManager::Release()
{
	for (CObject* pObject : m_ObjectList)
	{
		Safe_Delete(pObject);
	}
	m_ObjectList.clear();
}

void CObjectManager::Add_Object(CObject* _pObject)
{
	_pObject->Initialize();
	m_ObjectList.push_back(_pObject);
}
