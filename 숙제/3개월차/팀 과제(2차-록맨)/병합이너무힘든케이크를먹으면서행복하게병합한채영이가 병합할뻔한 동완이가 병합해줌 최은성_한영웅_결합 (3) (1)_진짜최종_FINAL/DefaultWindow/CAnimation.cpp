#include "pch.h"
#include "CAnimation.h"
#include "CScrollMgr.h"

void CAnimation::Update()
{
	//if (!m_vecAnInfo.empty())
	//{
	//	if (m_iCurFrame < m_vecAnInfo[0].iMaxFrame - 1)
	//	{
	//		if (m_ullTime + m_vecAnInfo[m_iCurFrame].iDuration < GetTickCount64())
	//		{
	//			++m_iCurFrame;
	//			m_ullTime = GetTickCount64();
	//		}
	//	}
	//	else
	//	{
	//		m_iCurFrame = 0;
	//	}
	//}
}

void CAnimation::Late_Update()
{
	//if (!m_vecAnInfo.empty())
	//{
	//	m_vecAnInfo[m_iCurFrame].tPosLT.fX = m_pTarget->Get_Info().fX - m_vecAnInfo[m_iCurFrame].tSize.fX * 0.5f;
	//	m_vecAnInfo[m_iCurFrame].tPosLT.fY = m_pTarget->Get_Info().fY + m_pTarget->Get_Info().fCY * 0.5f - m_vecAnInfo[m_iCurFrame].tSize.fY;
	//}
}

void CAnimation::Initialize()
{
	//m_tAnInfo = 
}

void CAnimation::Render(HDC _hDC, CObj* _pTarget)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	if (!m_vecAnInfo.empty())
	{
		m_vecAnInfo[m_iCurFrame].tPosLT.fX = _pTarget->Get_Info().fX - m_vecAnInfo[m_iCurFrame].tSize.fX * 0.5f;
		m_vecAnInfo[m_iCurFrame].tPosLT.fY = _pTarget->Get_Info().fY + _pTarget->Get_Info().fCY * 0.5f - m_vecAnInfo[m_iCurFrame].tSize.fY;
		if (m_iCurFrame < m_vecAnInfo[0].iMaxFrame)
		{
			GdiTransparentBlt(_hDC,										// 복사 받을 DC
				(int)(m_vecAnInfo[m_iCurFrame].tPosLT.fX + iScrollX),	// 복사 받을 위치 좌표 X, Y	
				(int)(m_vecAnInfo[m_iCurFrame].tPosLT.fY + iScrollY),
				(int)(m_vecAnInfo[m_iCurFrame].tSize.fX),	// 복사 받을 이미지의 가로, 세로
				(int)(m_vecAnInfo[m_iCurFrame].tSize.fY),
				m_hMemDC,												// 복사할 이미지 DC	
				(int)(m_vecAnInfo[m_iCurFrame].tImgLT.fX),	// 비트맵 출력 시작 좌표(Left, top)
				(int)(m_vecAnInfo[m_iCurFrame].tImgLT.fY),
				(int)(m_vecAnInfo[m_iCurFrame].tSize.fX),	// 복사할 이미지의 가로, 세로
				(int)(m_vecAnInfo[m_iCurFrame].tSize.fY),
				RGB(128, 0, 128));// 제거할 색상
			if (m_ullTime + m_vecAnInfo[m_iCurFrame].iDuration < GetTickCount64())
			{
				++m_iCurFrame;
				if (m_iCurFrame == m_vecAnInfo[0].iMaxFrame)
					m_iCurFrame = 0;
				m_ullTime = GetTickCount64();
			}
		}
	}
}

void CAnimation::Release()
{
}

void CAnimation::Set_Vector(ANINFO _AnInfo)
{
	for (int i = 0; i < _AnInfo.iMaxFrame; ++i)
	{
		m_vecAnInfo.push_back(
		ANINFO(
			FPOINT(_AnInfo.tImgLT.fX + i * _AnInfo.tSize.fX,0.f),		// 자를 이미지의 시작 LT 점
			FPOINT( // 이미지 출력 시 화면의 시작 LT 점
				_AnInfo.tTargetInfo.fX - _AnInfo.tSize.fX * 0.5f,	 // Left
				_AnInfo.tTargetInfo.fY + _AnInfo.tTargetInfo.fCY * 0.5f - _AnInfo.tSize.fY), // Top(기준점의 bottom - 이미지 세로길이)
			_AnInfo.tSize,												// 이미지 사이즈
			_AnInfo.tTargetInfo,										// 이미지 대상의 info(없어도 
			_AnInfo.iDuration,											// 1frame이 재생되는 시간(ms
			_AnInfo.iMaxFrame));										// 애니메이션 이미지 frame 수
	}
}

void CAnimation::Set_Manual_Vector(FPOINT _ImgLT, FPOINT _ImgSize, int _FrameIdx)
{
	if (!m_vecAnInfo.empty())
	{
		if (_FrameIdx >= m_vecAnInfo[0].iMaxFrame)
			return;
		m_vecAnInfo[_FrameIdx].tImgLT = _ImgLT;
		m_vecAnInfo[_FrameIdx].tSize = _ImgSize;			
	}
}

void CAnimation::Create_Animation()
{

}
