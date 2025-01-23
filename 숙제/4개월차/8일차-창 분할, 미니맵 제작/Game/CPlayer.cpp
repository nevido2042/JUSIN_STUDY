#include "pch.h"
#include "CPlayer.h"
#include "CDevice.h"
#include "CTextureMgr.h"

void CPlayer::Initialize()
{
}

int CPlayer::Update()
{
	m_tInfo.vPos.x += 1.f;
	m_tInfo.vPos.y += 1.f;

	return CObj::Update();
}

void CPlayer::Late_Update()
{
}

void CPlayer::Render()
{
		TCHAR	szBuf[MIN_STR] = L"";

		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Player");

		float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
		float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;

		D3DXVECTOR3	vTemp{ fCenterX, fCenterY, 0.f };

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, //����� �ؽ�ó �İ�ü
			nullptr,		// ����� �̹��� ������ ���� Rect �ּ�, null�� ��� �̹����� 0, 0�������� ���
			&vTemp,		// ����� �̹����� �߽� ��ǥ vec3 �ּ�, null�� ��� 0, 0 �̹��� �߽�
			&m_tInfo.vPos,		// ��ġ ��ǥ�� ���� vec3 �ּ�, null�� ��� ��ũ�� �� 0, 0 ��ǥ ���	
			D3DCOLOR_ARGB(255, 255, 255, 255)); // ����� �̹����� ���� ���� ��, 0xffffffff�� �Ѱ��ָ� ���� �ʰ� ���� ���� ����

		swprintf_s(szBuf, L"Player");
		RECT tRect{ (long)m_tInfo.vPos.x, (long)m_tInfo.vPos.y, (long)m_tInfo.vPos.x + 100, (long)m_tInfo.vPos.x + 100 };
		CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
			szBuf,		// ����� ���ڿ�
			lstrlen(szBuf),  // ���ڿ� ������ ũ��
			&tRect,	// ����� ��Ʈ ��ġ
			0,			// ���� ����(�ɼ�)
			D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CPlayer::Release()
{
}


