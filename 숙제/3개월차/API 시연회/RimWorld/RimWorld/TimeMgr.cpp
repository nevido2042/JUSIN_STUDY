#include "pch.h"
#include "TimeMgr.h"

CTimeMgr* CTimeMgr::m_pInstance = nullptr;

CTimeMgr::CTimeMgr()
	:m_iCurFrame(0)
{

}

CTimeMgr::~CTimeMgr()
{
}

void CTimeMgr::Initialize()
{	
	m_iCurFrame = 0;
}

void CTimeMgr::Update()
{
	++m_iCurFrame;
}
