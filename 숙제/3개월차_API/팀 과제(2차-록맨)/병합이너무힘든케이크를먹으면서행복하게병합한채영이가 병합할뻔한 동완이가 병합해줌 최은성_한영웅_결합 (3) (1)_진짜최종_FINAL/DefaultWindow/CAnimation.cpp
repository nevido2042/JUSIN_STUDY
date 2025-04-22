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
				RGB(128, 0, 128));// ������ ����
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
			FPOINT(_AnInfo.tImgLT.fX + i * _AnInfo.tSize.fX,0.f),		// �ڸ� �̹����� ���� LT ��
			FPOINT( // �̹��� ��� �� ȭ���� ���� LT ��
				_AnInfo.tTargetInfo.fX - _AnInfo.tSize.fX * 0.5f,	 // Left
				_AnInfo.tTargetInfo.fY + _AnInfo.tTargetInfo.fCY * 0.5f - _AnInfo.tSize.fY), // Top(�������� bottom - �̹��� ���α���)
			_AnInfo.tSize,												// �̹��� ������
			_AnInfo.tTargetInfo,										// �̹��� ����� info(��� 
			_AnInfo.iDuration,											// 1frame�� ����Ǵ� �ð�(ms
			_AnInfo.iMaxFrame));										// �ִϸ��̼� �̹��� frame ��
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
