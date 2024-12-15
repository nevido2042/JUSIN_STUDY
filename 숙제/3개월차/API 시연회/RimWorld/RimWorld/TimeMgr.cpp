#include "pch.h"
#include "TimeMgr.h"

CTimeMgr* CTimeMgr::m_pInstance = nullptr;

CTimeMgr::CTimeMgr()
	:m_iCurFrame(0), m_fGameSpeed(0.f)
{

}

CTimeMgr::~CTimeMgr()
{
}

void CTimeMgr::Initialize()
{	
	m_iCurFrame = 0;
	m_fGameSpeed = 1.f;
}

void CTimeMgr::Update()
{
	++m_iCurFrame;
}
