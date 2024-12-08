#include "pch.h"
#include "DummyMgr.h"

CDummyMgr* CDummyMgr::m_pInstance = nullptr;

CDummyMgr::CDummyMgr()
{
}

CDummyMgr::~CDummyMgr()
{
	Release();
}

void CDummyMgr::Release()
{
}
