#include "pch.h"
#include "KBossMonster.h"



KBossMonster::KBossMonster() : m_ulPatternTime(GetTickCount64()), m_iPattern(0), m_fDistance(0)
{
}

void KBossMonster::Initialize()
{
	m_tInfo.fCX = 200.f;
	m_tInfo.fCY = 100.f;
	m_fSpeed = 1.f;

	m_tInfo.fX = 300.f;
	m_tInfo.fY = 150.f;

	m_iHp = 1000;
	m_iDamage = 1;
	m_fDistance = 300.f; // ui���� ���� 

}

int KBossMonster::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	m_tInfo.fX += m_fSpeed;

	
	if (m_ulPatternTime + 2500 < GetTickCount64()) // 2.5�ʸ��� ���� �ٲٰԲ�
	{

		m_iPattern = (m_iPattern + 1) % 5; // ���� ���� ��� �ȳ����� ����. 
		
		m_ulPatternTime = GetTickCount64();
	}

	

	Obj::Update_Rect();

	

    return OBJ_NOEVENT;
}

void KBossMonster::Late_Update()
{
	

	if (m_iHp <= 0)
		m_bDead = true;

	if (GAME_WIN_LEFT >= m_tRect.left || WINCX - m_tInfo.fCX  <= m_tRect.right) // ���� ui�� �°� ���� �ʿ�
		m_fSpeed *= -1.f;

	//2����
	//if (m_tRect.right <= (WINCX - 200.f) / 2.f )
	//{
	//	if (GAME_WIN_LEFT >= m_tRect.left || WINCX - m_tInfo.fCX - (WINCX - 200.f) / 2.f <= m_tRect.right) // ���� ui�� �°� ���� �ʿ�
	//		m_fSpeed *= -1.f;
	//}
	//else
	//{
	//	if (GAME_WIN_LEFT + (WINCX - 200.f) / 2.f >= m_tRect.left || WINCX - m_tInfo.fCX - 100 <= m_tRect.right) // ���� ui�� �°� ���� �ʿ�
	//		m_fSpeed *= -1.f;
	//}
	

	

	if (!m_bDead)
	{

		switch (m_iPattern)
		{
		case 0:
			if (m_ulTime + 300 < GetTickCount64())
			{
				Shoot_Sun();
				m_ulTime = GetTickCount64();
			}
			break;
		case 1:
			if (m_ulTime + 50 < GetTickCount64())
			{
				Shoot_Accel();
				m_ulTime = GetTickCount64();
			}
			break;
		case 2:
			if (m_ulTime + 10 < GetTickCount64())
			{
				Shoot_Sprial();
				m_ulTime = GetTickCount64();
			}
			break;
		case 3:
			if (m_ulTime + 100 < GetTickCount64())
			{
				Shoot_Rain();
				m_ulTime = GetTickCount64();
			}
			break;
		case 4:
			if (m_ulTime + 300 < GetTickCount64())
			{
				Shoot_Sun_Screw();
				m_ulTime = GetTickCount64();
			}
			break;
		default:
			break;
		}
		

	}
}

void KBossMonster::Release()
{
}

void KBossMonster::Render(HDC _hdc)
{
	HBRUSH myBrush = (HBRUSH)CreateSolidBrush(RGB(102, 51, 255));
	HBRUSH oldBrush = (HBRUSH)SelectObject(_hdc, myBrush);
	Rectangle(_hdc, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
	SelectObject(_hdc, oldBrush);
	DeleteObject(myBrush);
}

void KBossMonster::Shoot_Sun()
{
	for (int i = 0; i <= 24; ++i)
	{
		m_pBulletList->push_back(Create_Bullet());
		m_pBulletList->back()->Set_Angle(float(i * 15));
		m_pBulletList->back()->Set_Pos(m_tInfo.fX, m_tInfo.fY);
		m_pBulletList->back()->Set_Damage(1); // ���� ���� �����ַ��� 1�� ����
	}
}

void KBossMonster::Shoot_Accel()
{
	// �����ϴ�
	
	m_pBulletList->push_back(Create_Bullet());
	m_pBulletList->back()->Set_Pos(m_tRect.left, m_tInfo.fY + 50);
	Calc_Angle(m_tRect.left, m_tInfo.fY + 50);
	m_pBulletList->back()->Set_Angle(m_fAngle);
	m_pBulletList->back()->Set_Accel(0.2f);
	m_pBulletList->back()->Set_Damage(1); // ���� ���� �����ַ��� 1�� ����

	m_pBulletList->push_back(Create_Bullet());
	m_pBulletList->back()->Set_Pos(m_tInfo.fX, m_tInfo.fY + 50);
	Calc_Angle(m_tInfo.fX, m_tInfo.fY + 50);
	m_pBulletList->back()->Set_Angle(m_fAngle);
	m_pBulletList->back()->Set_Accel(0.2f);
	m_pBulletList->back()->Set_Damage(1); // ���� ���� �����ַ��� 1�� ����

	m_pBulletList->push_back(Create_Bullet());
	m_pBulletList->back()->Set_Pos(m_tRect.right, m_tInfo.fY + 50);
	Calc_Angle(m_tRect.right, m_tInfo.fY + 50);
	m_pBulletList->back()->Set_Angle(m_fAngle);
	m_pBulletList->back()->Set_Accel(0.2f);
	m_pBulletList->back()->Set_Damage(1); // ���� ���� �����ַ��� 1�� ����


	
}

void KBossMonster::Shoot_Sprial()
{
	
		m_pBulletList->push_back(Create_Bullet());
		m_pBulletList->back()->Set_Angle(m_fAngle);
		m_pBulletList->back()->Set_Pos(m_tInfo.fX + m_fSpeed * cosf(m_fAngle), m_tInfo.fY - m_fSpeed * sinf(m_fAngle));
		m_pBulletList->back()->Set_Damage(1); // ���� ���� �����ַ��� 1�� ����

		Set_Angle(m_fAngle + 15.f);
	}

void KBossMonster::Shoot_Rain()
{
	for (int i = 0; i < 5; ++i)
	{
		m_pBulletList->push_back(Create_Bullet());
		m_pBulletList->back()->Set_Angle(270.f);
		m_pBulletList->back()->Set_Pos(float(m_tInfo.fX - 50 + 25 * i ), float(m_tInfo.fY + 50) );
		m_pBulletList->back()->Set_Damage(1);; // ���� ���� �����ַ��� 1�� ����

	}
	

}

void KBossMonster::Shoot_Sun_Screw()
{
	for (int i = 0; i <= 24; ++i)
	{
		m_pBulletList->push_back(Create_Screw_Bullet(5.f));
		m_pBulletList->back()->Set_Angle(float(i * 15));
		m_pBulletList->back()->Set_Pos(m_tInfo.fX, m_tInfo.fY);
		m_pBulletList->back()->Set_Damage(1); // ���� ���� �����ַ��� 1�� ����
	}
}

void KBossMonster::Calc_Angle(float fX, float fY)
{
	float   fWidth(0.f), fHeight(0.f), fDiagonal(0.f), fRadian(0.f);

	fWidth = m_pTarget->Get_Info().fX - fX;
	fHeight = m_pTarget->Get_Info().fY - fY;

	fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

	fRadian = acosf(fWidth / fDiagonal);

	m_fAngle = fRadian * (180.f / PI);

	if (m_pTarget->Get_Info().fY > fY)
		m_fAngle *= -1.f;
}


