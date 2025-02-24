#include "pch.h"
#include "CPlayer.h"
#include "CBullet.h"
#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CLineMgr.h"
#include "CKeyMgr.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CSoundMgr.h"
#include "CNetwork.h"

float	g_fVolume(1.f);

CPlayer::CPlayer() 
: m_bJump(false), m_fJumpPower(0.f), m_fTime(0.f), m_bStretch(false)
, m_dwTime(GetTickCount()), m_iID(0), m_bMoveRight(false), m_bMoveLeft(false)
{
	ZeroMemory(&m_tPosin, sizeof(POINT));
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_tInfo  = { 100.f, WINCY / 2.f, 200.f, 200.f };
	m_fSpeed = 1.f;
	m_fDistance = 100.f;

	m_fJumpPower = 20.f;

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_DOWN.bmp",  L"Player_DOWN");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_UP.bmp",	 L"Player_UP");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_LEFT.bmp",	 L"Player_LEFT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_RIGHT.bmp", L"Player_RIGHT");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_LU.bmp", L"Player_LU");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_RU.bmp", L"Player_RU");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_LD.bmp", L"Player_LD");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_RD.bmp", L"Player_RD");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Stretch.bmp", L"Stretch");

	m_pImgKey = L"Player_DOWN";
	m_eCurState = IDLE;
	m_ePreState = IDLE;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iMotion = 0;
	m_tFrame.dwSpeed = 200;
	m_tFrame.dwTime = GetTickCount();

	m_eRender = RENDER_GAMEOBJECT;

	//CSoundMgr::Get_Instance()->PlayBGM(L"Success.wav", g_fVolume);
}




int CPlayer::Update()
{
	if (m_bDead)
	{
		return OBJ_DEAD;
	}

	if (m_bMoveRight)
	{
		m_tInfo.fX += m_fSpeed;
		m_pImgKey = L"Player_RIGHT";
		m_eCurState = WALK;
		m_bStretch = false;
	}
	if (m_bMoveLeft)
	{
		m_tInfo.fX -= m_fSpeed;
		m_pImgKey = L"Player_RIGHT";
		m_eCurState = WALK;
		m_bStretch = true;
	}

	Key_Input();
	Change_Motion();

	__super::Update_Rect();

	return OBJ_NOEVENT;
}
void CPlayer::Late_Update()
{
	Jumping();
	Offset();	
	__super::Move_Frame();

#ifdef  _DEBUG

	/*if (m_dwTime + 1000 < GetTickCount())
	{
		cout << "플레이어 위치 : " << m_tInfo.fX << '\t' << m_tInfo.fY << endl;
		m_dwTime = GetTickCount();
	}*/


#endif //  _DEBUG


}


void CPlayer::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
	HDC		hStretchDC = CBmpMgr::Get_Instance()->Find_Image(L"Stretch");

	if (!m_bStretch)
	{
		GdiTransparentBlt(hDC,			// 복사 받을 DC
			m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
			m_tRect.top + iScrollY,
			(int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
			(int)m_tInfo.fCY,
			hMemDC,						// 복사할 이미지 DC	
			(int)m_tInfo.fCX * m_tFrame.iFrameStart,							// 비트맵 출력 시작 좌표(Left, top)
			(int)m_tInfo.fCY * m_tFrame.iMotion,
			(int)m_tInfo.fCX,			// 복사할 이미지의 가로, 세로
			(int)m_tInfo.fCY,
			RGB(0, 0, 0));		// 제거할 색상
	}

	else
	{
		StretchBlt(hStretchDC, 
			0, 0,
			(int)m_tInfo.fCX,
			(int)m_tInfo.fCY,
			hMemDC,
			int(m_tInfo.fCX * m_tFrame.iFrameStart + m_tInfo.fCX),							// 비트맵 출력 시작 좌표(Left, top)
			(int)m_tInfo.fCY * m_tFrame.iMotion,
			(int)-m_tInfo.fCX,
			(int)m_tInfo.fCY,
			SRCCOPY);


		GdiTransparentBlt(hDC,			// 복사 받을 DC
			m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
			m_tRect.top + iScrollY,
			(int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
			(int)m_tInfo.fCY,
			hStretchDC,						// 복사할 이미지 DC	
			0,
			0,
			(int)m_tInfo.fCX,			// 복사할 이미지의 가로, 세로
			(int)m_tInfo.fCY,
			RGB(0, 0, 0));		// 제거할 색상
	}

	
}

void CPlayer::Release()
{
}

void CPlayer::Key_Input()
{
	if (g_hWnd != GetForegroundWindow())
	{
		return;
	}

	float	fY(0.f);

	if (CKeyMgr::Get_Instance()->Key_Down(VK_LEFT))
	{
		MSG_MOVE_LEFT_PLAYER msgMoveLeft;
		msgMoveLeft.type = MOVE_LEFT_PLAYER;

		//CNetwork::Get_Instance()->Send_Message((MSG_ID&)msgMoveLeft);

		//-------------------------------------------------//

		CNetwork::Get_Instance()->Enqueue_SendQ((MSG_ID&)msgMoveLeft, sizeof(MSG_MOVE_LEFT_PLAYER));

		/*m_tInfo.fX -= m_fSpeed;
		m_pImgKey = L"Player_RIGHT";
		m_eCurState = WALK;
		m_bStretch = true;*/
	}
	else if (CKeyMgr::Get_Instance()->Key_Down(VK_RIGHT))
	{
		MSG_MOVE_RIGHT_PLAYER msgMoveRight;
		msgMoveRight.type = MOVE_RIGHT_PLAYER;

		//CNetwork::Get_Instance()->Send_Message((MSG_ID&)msgMoveRight);
		CNetwork::Get_Instance()->Enqueue_SendQ((MSG_ID&)msgMoveRight, sizeof(MSG_MOVE_RIGHT_PLAYER));

		/*m_tInfo.fX += m_fSpeed;
		m_pImgKey = L"Player_RIGHT";
		m_eCurState = WALK;
		m_bStretch = false;*/
	}
	else if (CKeyMgr::Get_Instance()->Key_Up(VK_RIGHT))
	{
		//오른쪽 가지 말라 메시지 요청

		MSG_STOP_RIGHT_PLAYER msgStopRight;
		msgStopRight.type = STOP_RIGHT_PLAYER;

		//CNetwork::Get_Instance()->Send_Message((MSG_ID&)msgStopRight);
		CNetwork::Get_Instance()->Enqueue_SendQ((MSG_ID&)msgStopRight, sizeof(MSG_MOVE_RIGHT_PLAYER));

	}
	else if (CKeyMgr::Get_Instance()->Key_Up(VK_LEFT))
	{
		MSG_STOP_LEFT_PLAYER msgStopLeft;
		msgStopLeft.type = STOP_LEFT_PLAYER;

		CNetwork::Get_Instance()->Enqueue_SendQ((MSG_ID&)msgStopLeft, sizeof(MSG_STOP_LEFT_PLAYER));
		//CNetwork::Get_Instance()->Send_Message((MSG_ID&)msgStopLeft);
	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
	{
		/*m_tInfo.fY -= m_fSpeed;
		m_pImgKey = L"Player_UP";
		m_eCurState = WALK;
		m_bStretch = false;*/
	}

	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
	{
		/*m_tInfo.fY += m_fSpeed;
		m_pImgKey = L"Player_DOWN";
		m_eCurState = WALK;
		m_bStretch = false;*/
	}

	if (CKeyMgr::Get_Instance()->Key_Up(VK_SPACE))
	{
		// m_bJump = true;
		// m_bStretch = false;

		CSoundMgr::Get_Instance()->PlaySound(L"Success.wav", SOUND_EFFECT, g_fVolume);
	}

	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_F1))
	{	
		g_fVolume -= 0.1f;

		if (0.f > g_fVolume)
			g_fVolume = 0.f;

		CSoundMgr::Get_Instance()->SetChannelVolume(SOUND_EFFECT, g_fVolume);
		return;
	}

	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_F2))
	{
		g_fVolume += 0.1f;

		if (1.f < g_fVolume)
			g_fVolume = 1.f;

		CSoundMgr::Get_Instance()->SetChannelVolume(SOUND_EFFECT, g_fVolume);
		return;
	}

	else
		m_eCurState = IDLE;
}

void CPlayer::Jumping()
{
	float	fY(0.f);

	bool	bLineCol = CLineMgr::Get_Instance()->Collision_Line(m_tInfo.fX, &fY);

	if (m_bJump)
	{
		m_tInfo.fY -= (m_fJumpPower * sinf(45.f) * m_fTime) - (9.8f * m_fTime * m_fTime) * 0.5f;
		m_fTime += 0.2f;

		if (bLineCol && (fY < m_tInfo.fY))
		{
			m_bJump = false;
			m_fTime = 0.f;
			m_tInfo.fY = fY;
		}
	}
	else if (bLineCol)
	{
		m_tInfo.fY = fY;
	}
}

void CPlayer::Offset()
{

	int		iOffSetminX = 100;
	int		iOffSetmaxX = 700;

	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();

	if (iOffSetminX > m_tInfo.fX + iScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(m_fSpeed);

	if (iOffSetmaxX < m_tInfo.fX + iScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(-m_fSpeed);

	int		iOffSetminY = 100;
	int		iOffSetmaxY = 500;

	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	if (iOffSetminY > m_tInfo.fY + iScrollY)
		CScrollMgr::Get_Instance()->Set_ScrollY(m_fSpeed);

	if (iOffSetmaxY < m_tInfo.fY + iScrollY)
		CScrollMgr::Get_Instance()->Set_ScrollY(-m_fSpeed);
}

CObj* CPlayer::Create_Shield()
{
	CObj* pShield = CAbstractFactory<CShield>::Create();
	
	pShield->Set_Target(this);

	return pShield;
}

void CPlayer::Change_Motion()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case CPlayer::IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 0;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			break;

		case CPlayer::WALK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 1;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			break;

		case CPlayer::ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iMotion = 2;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			break;

		case CPlayer::HIT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iMotion = 3;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			break;

		case CPlayer::DEAD:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iMotion = 4;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount();
			break;	
		}

		m_ePreState = m_eCurState;
	}

}


