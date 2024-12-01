#include "pch.h"
#include "BulletMonster.h"
//#include "BulletOne.h"
#include "MonsterBullet.h"

BulletMonster::BulletMonster()
	:m_pBulletList(nullptr), m_ullLastShot(0)
{
}

BulletMonster::~BulletMonster()
{
}

void BulletMonster::CreateBullet()
{
	float fAngle(0.f), fDiagonal(0.f), fWidth(0.f),fHeight(0.f), fRadian(0.f);

	fWidth = m_pTarget->Get_Info().fX - Get_Info().fX;
	fHeight = m_pTarget->Get_Info().fY - Get_Info().fY;
	fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

	fRadian = acos(fWidth / fDiagonal);
	fAngle = fRadian * (180 / PI);
	if (m_pTarget->Get_Info().fY > Get_Info().fY)
	{
		fAngle = 360.f - fAngle;
	}

	m_pBulletList->push_back(new MonsterBullet(fAngle));
	m_pBulletList->back()->Initialize();
	m_pBulletList->back()->Set_Pos(m_tInfo.fX, m_tInfo.fY);
}

void BulletMonster::Initialize()
{
	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 30.f;
	m_fSpeed = 1.f;

	m_iHp = 10;
	m_iDamage = 1;

	m_iDir = rand() % 4;
	int iXorY = 0;

	switch (m_iDir)
	{
	case LEFT:
		iXorY = rand() % int(GAME_WINCY + GAME_WIN_TOP);
		m_tInfo.fX = int(GAME_WIN_LEFT) + m_tInfo.fCX;
		m_tInfo.fY = (float)iXorY;
		break;
	case TOP:
		iXorY = rand() % int(GAME_WINCX + GAME_WIN_LEFT);
		m_tInfo.fY = int(GAME_WIN_TOP) + m_tInfo.fCY;
		m_tInfo.fX = (float)iXorY;
		break;
	case RIGHT:
		iXorY = rand() % int(GAME_WINCY + GAME_WIN_TOP);
		m_tInfo.fX = int(GAME_WIN_RIGHT) - m_tInfo.fCX;
		m_tInfo.fY = (float)iXorY;
		break;
	case BOTTOM:
		iXorY = rand() % int(GAME_WINCX + GAME_WIN_LEFT);
		m_tInfo.fY = int(GAME_WIN_BOTTOM) - m_tInfo.fCY;
		m_tInfo.fX = (float)iXorY;
		break;
	case NODIR:
		break;
	default:
		break;
	}
}

int BulletMonster::Update()
{
	if (m_bDead) {
		return OBJ_DEAD;
	}

	//m_fSpeed += 0.0005f;

	float   fWidth(0.f), fHeight(0.f), fDiagonal(0.f), fRadian(0.f);

	fWidth = m_pTarget->Get_Info().fX - m_tInfo.fX;
	fHeight = m_pTarget->Get_Info().fY - m_tInfo.fY;

	fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

	fRadian = acosf(fWidth / fDiagonal);

	m_fAngle = fRadian * (180.f / PI);

	if (m_pTarget->Get_Info().fY > m_tInfo.fY)
		m_fAngle *= -1.f;

	// degree to radian
	m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f));
	m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * (PI / 180.f));

	if(m_ullLastShot + 1000 < GetTickCount64())//시간 마다
	{
		//플레이어의 좌표를 받아서 그 방향으로 발사
		CreateBullet();
		m_ullLastShot = GetTickCount64();
	}

	Obj::Update_Rect();

	return OBJ_NOEVENT;
}

void BulletMonster::Late_Update()
{
	if (m_iHp <= 0) {
		m_bDead = true;
	}

}
