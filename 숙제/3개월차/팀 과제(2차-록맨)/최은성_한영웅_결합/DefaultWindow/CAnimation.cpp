#include "pch.h"
#include "CAnimation.h"
#include "CScrollMgr.h"

void CAnimation::Update()
{
	if (!m_vecAnInfo.empty())
	{
		if (m_iCurFrame < m_vecAnInfo[0].iMaxFrame - 1)
		{
			if (m_ullTime + m_vecAnInfo[m_iCurFrame].iDuration < GetTickCount64())
			{
				++m_iCurFrame;
				m_ullTime = GetTickCount64();
			}
		}
		else
		{
			m_iCurFrame = 0;
		}
	}
}

void CAnimation::Late_Update()
{
	if (!m_vecAnInfo.empty())
	{
		m_vecAnInfo[m_iCurFrame].tPosLT.fX = m_pTarget->Get_Info().fX - m_vecAnInfo[m_iCurFrame].tSize.fX * 0.5f;
		m_vecAnInfo[m_iCurFrame].tPosLT.fY = m_pTarget->Get_Info().fY - m_vecAnInfo[m_iCurFrame].tSize.fY * 0.5f;
	}
}

void CAnimation::Initialize()
{
	//m_tAnInfo = 
}

void CAnimation::Render(HDC _hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	if (!m_vecAnInfo.empty())
	{

		GdiTransparentBlt(_hDC,										// ���� ���� DC
			(int)(m_vecAnInfo[m_iCurFrame].tPosLT.fX + iScrollX),	// ���� ���� ��ġ ��ǥ X, Y	
			(int)(m_vecAnInfo[m_iCurFrame].tPosLT.fY + iScrollY),
			(int)(m_vecAnInfo[m_iCurFrame].tSize.fX),	// ���� ���� �̹����� ����, ����
			(int)(m_vecAnInfo[m_iCurFrame].tSize.fY),
			m_hMemDC,												// ������ �̹��� DC	
			(int)(m_vecAnInfo[m_iCurFrame].tImgLT.fX),	// ��Ʈ�� ��� ���� ��ǥ(Left, top)
			(int)(m_vecAnInfo[m_iCurFrame].tImgLT.fY),
			(int)(m_vecAnInfo[m_iCurFrame].tSize.fX),	// ������ �̹����� ����, ����
			(int)(m_vecAnInfo[m_iCurFrame].tSize.fY),
			RGB(128, 0, 128));									    // ������ ����
	}
}

void CAnimation::Release()
{
}

void CAnimation::Set_Vector(ANINFO _AnInfo)
{
	for (int i = 0; i < _AnInfo.iMaxFrame; ++i)
	{
		m_vecAnInfo.push_back(ANINFO(FPOINT(_AnInfo.tImgLT.fX + i * _AnInfo.tSize.fX, 0.f),
									 FPOINT(m_pTarget->Get_Info().fX - _AnInfo.tSize.fX * 0.5f,
											m_pTarget->Get_Info().fY - _AnInfo.tSize.fY * 0.5f),
									 _AnInfo.tSize,
									 _AnInfo.tTargetInfo,
									 _AnInfo.iDuration,
									 _AnInfo.iMaxFrame));
	}
}

void CAnimation::Create_Animation()
{
	
}
