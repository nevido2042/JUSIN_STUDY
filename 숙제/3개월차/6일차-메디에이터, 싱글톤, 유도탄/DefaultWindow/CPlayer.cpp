#include "pch.h"
#include "CPlayer.h"
#include "CBullet.h"
#include "CAbstractFactory.h"
#include "CObjMgr.h"

CPlayer::CPlayer() 
	:m_pPath(nullptr), m_bFollowingPath(false), m_fSlope(0.f)
{
	ZeroMemory(&m_tPosin, sizeof(POINT));
	//ZeroMemory(&m_tTargetPoint, sizeof(POINT));
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_tInfo  = { 100.f, WINCY / 2.f, 100.f, 100.f };
	m_fSpeed = 3.f;
	m_fDistance = 100.f;
}

void CPlayer::Start()
{
	m_pPath = CObjMgr::Get_Instance()->Get_Path();
	//점을 하나 받는다.
	m_IterTargetPoint = static_cast<CPath*>(m_pPath)->Get_PointList()->begin();
	if (m_IterTargetPoint == static_cast<CPath*>(m_pPath)->Get_PointList()->end())
	{
		m_bFollowingPath = false;
	}
	else
	{
		m_bFollowingPath = true;
		m_fSlope = abs((m_IterTargetPoint->y - Get_Info().fY) / (m_IterTargetPoint->x - Get_Info().fX));
	}
}

int CPlayer::Update()
{
	//Key_Input();

	// CObj::Update_Rect();
	__super::Update_Rect();

	return OBJ_NOEVENT;
}
void CPlayer::Late_Update()
{
	// degree to radian

	m_tPosin.x = long(m_tInfo.fX + (m_fDistance * cosf(m_fAngle * (PI / 180.f))));
	m_tPosin.y = long(m_tInfo.fY - (m_fDistance * sinf(m_fAngle * (PI / 180.f))));


	if (m_bFollowingPath)
	{
		m_bFollowingPath = Follow_Path();
	}
}


void CPlayer::Render(HDC hDC)
{
	Rectangle(hDC, 
		m_tRect.left, 
		m_tRect.top, 
		m_tRect.right, 
		m_tRect.bottom);

	// 포신
	MoveToEx(hDC, (int)m_tInfo.fX, (int)m_tInfo.fY, nullptr);
	LineTo(hDC, m_tPosin.x, m_tPosin.y);
}

void CPlayer::Release()
{
}

void CPlayer::Key_Input()
{
	// GetKeyState
	// GetAsyncKeyState // 함수 호출 시 반환되는 값에 따른 키 값 처리 가능

#pragma region 복습

	/*if (GetAsyncKeyState(VK_RIGHT))
	{
		if (GetAsyncKeyState(VK_UP))
		{
			m_tInfo.fX += m_fSpeed / sqrtf(2.f);
			m_tInfo.fY -= m_fSpeed / sqrtf(2.f);
		}

		else if (GetAsyncKeyState(VK_DOWN))
		{
			m_tInfo.fX += m_fSpeed / sqrtf(2.f);
			m_tInfo.fY += m_fSpeed / sqrtf(2.f);
		}

		else
		{
			m_tInfo.fX += m_fSpeed;
		}

	}

	else if (GetAsyncKeyState(VK_LEFT))
	{
		if (GetAsyncKeyState(VK_UP))
		{
			m_tInfo.fX -= m_fSpeed / sqrtf(2.f);
			m_tInfo.fY -= m_fSpeed / sqrtf(2.f);
		}

		else if (GetAsyncKeyState(VK_DOWN))
		{
			m_tInfo.fX -= m_fSpeed / sqrtf(2.f);
			m_tInfo.fY += m_fSpeed / sqrtf(2.f);
		}

		else
		{
			m_tInfo.fX -= m_fSpeed;
		}
	}

	else if (GetAsyncKeyState(VK_UP))
	{
		m_tInfo.fY -= m_fSpeed;
	}

	else if (GetAsyncKeyState(VK_DOWN))
	{
		m_tInfo.fY += m_fSpeed;
	}

	else if (GetAsyncKeyState(VK_SPACE))
	{
		m_pBullet->push_back(Create_Bullet(DIR_UP));
		m_pBullet->push_back(Create_Bullet(DIR_LU));
		m_pBullet->push_back(Create_Bullet(DIR_RU));
	}

	if (GetAsyncKeyState('W')) m_pBullet->push_back(Create_Bullet(DIR_UP));
	if (GetAsyncKeyState('S')) m_pBullet->push_back(Create_Bullet(DIR_DOWN));
	if (GetAsyncKeyState('A'))
	{
		m_pBullet->push_back(Create_Bullet(DIR_LEFT));
	}

	if (GetAsyncKeyState('D'))
	{
		m_pBullet->push_back(CAbstractFactory<CBullet>::Create(m_tInfo.fX, m_tInfo.fY, DIR_RIGHT));
	}*/
#pragma endregion
	if (GetAsyncKeyState(VK_UP))
	{
		m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f));
		m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * (PI / 180.f));
	}

	if (GetAsyncKeyState(VK_DOWN))
	{
		m_tInfo.fX += -m_fSpeed * cosf(m_fAngle * (PI / 180.f));
		m_tInfo.fY -= -m_fSpeed * sinf(m_fAngle * (PI / 180.f));
	}
	

	if (GetAsyncKeyState(VK_LEFT))
	{
		m_fAngle += 5.f;
	}

	if (GetAsyncKeyState(VK_RIGHT))
	{
		m_fAngle -= 5.f;
	}

	if (GetAsyncKeyState(VK_SPACE))
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ_BULLET, CAbstractFactory<CGuideBullet>::
			Create((float)m_tPosin.x, (float)m_tPosin.y, m_fAngle));
	}

	if (GetAsyncKeyState(VK_F1))
	{
		CObjMgr::Get_Instance()->Add_Object(OBJ_SHIELD, Create_Shield());
	}

}

CObj* CPlayer::Create_Shield()
{
	CObj* pShield = CAbstractFactory<CShield>::Create();
	
	pShield->Set_Target(this);

	return pShield;
}

bool CPlayer::Follow_Path()
{
	float fWidth = Get_Info().fX - m_IterTargetPoint->x;
	float fHeight = Get_Info().fY - m_IterTargetPoint->y;
	float fDist = sqrtf(fWidth * fWidth + fHeight * fHeight);
	//목표 점에 도달 했는가?
	if (fDist <= m_fSpeed * m_fSlope)
	{
		//도달 했으면 새로운 목표를 설정하라
		++m_IterTargetPoint;

		if (m_IterTargetPoint != static_cast<CPath*>(m_pPath)->Get_PointList()->end())
		{
			m_fSlope = abs((m_IterTargetPoint->y - Get_Info().fY) / (m_IterTargetPoint->x - Get_Info().fX));


			return true;
		}
		else
		{
			//더 이상 갈 목표가 없으면 길따라가기 종료
			return false;
		}
	}

	//기울기를 구하고
	//속도만큼 fx를 증가시며 기울기 보정으로 y도 증가시켜라

	//float fSlope(0.f);
	if (m_IterTargetPoint->y > m_tInfo.fY)
	{
		m_tInfo.fY += m_fSlope * m_fSpeed;
	}
	else
	{
		m_tInfo.fY -= m_fSlope * m_fSpeed;
	}

	//m_tInfo.fY -= m_fSlope * m_fSpeed;
		
	if (m_IterTargetPoint->x > m_tInfo.fX)
	{
		m_tInfo.fX += m_fSpeed;
	}
	else
	{
		m_tInfo.fX -= m_fSpeed;
	}


	return true;

}


