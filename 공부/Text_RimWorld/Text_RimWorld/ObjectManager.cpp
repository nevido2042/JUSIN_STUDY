#include "pch.h"
#include "ObjectManager.h"
#include "Define.h"
#include "Rim.h"

CObjectManager::~CObjectManager()
{
	Release();
}

void CObjectManager::Initialize()
{
}

void CObjectManager::Update()
{
	int i = 0;
	for (CObject* pObject : m_ObjectList)
	{
		pObject->Update();
		if (CRim* pRim = dynamic_cast<CRim*>(pObject))
		{
			pRim->Print_State(i);
			++i;
		}
	}
}

void CObjectManager::Render()
{
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
