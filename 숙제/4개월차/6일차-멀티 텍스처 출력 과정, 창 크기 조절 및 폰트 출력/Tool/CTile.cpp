#include "pch.h"
#include "CTile.h"
#include "CTextureMgr.h"

CTile::CTile()
{
}

CTile::~CTile()
{
}

void CTile::Initialize()
{
	m_pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", 7);
}

void CTile::Render(CDevice* _pDevice)
{
	float	fCenterX = m_pTexInfo->tImgInfo.Width / 2.f;
	float	fCenterY = m_pTexInfo->tImgInfo.Height / 2.f;

	D3DXVECTOR3	vTemp{ fCenterX, fCenterY, 0.f };

	_pDevice->Get_Sprite()->Draw(m_pTexInfo->pTexture, //����� �ؽ�ó �İ�ü
		nullptr,		// ����� �̹��� ������ ���� Rect �ּ�, null�� ��� �̹����� 0, 0�������� ���
		&vTemp,		// ����� �̹����� �߽� ��ǥ vec3 �ּ�, null�� ��� 0, 0 �̹��� �߽�
		&m_tInfo.vPos,		// ��ġ ��ǥ�� ���� vec3 �ּ�, null�� ��� ��ũ�� �� 0, 0 ��ǥ ���	
		D3DCOLOR_ARGB(255, 255, 255, 255)); // ����� �̹����� ���� ���� ��, 0xffffffff�� �Ѱ��ָ� ���� �ʰ� ���� ���� ����

	// ��Ʈ�� ��� ��ġ�� ���� ����
	/*D3DXMATRIX matTrans;
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);
	_pDevice->Get_Sprite()->SetTransform(&matTrans);*/

	RECT tRect = { (LONG)(m_tInfo.vPos.x - TILECX * 0.5f), (LONG)(m_tInfo.vPos.y - TILECY * 0.5f),
		LONG(m_tInfo.vPos.x + TILECX * 0.5f), LONG(m_tInfo.vPos.y + TILECY * 0.5f) };

	TCHAR	szBuf[MIN_STR] = L"";
	int	iNumber = 1000;
	swprintf_s(szBuf, L"%d", iNumber);

	_pDevice->Get_Font()->DrawTextW(_pDevice->Get_Sprite(),
		szBuf,											// ����� ���ڿ�
		lstrlen(szBuf),									// ���ڿ� ������ ũ��
		&tRect,											// ����� ��Ʈ ��ġ
		DT_CENTER | DT_VCENTER /*| DT_SINGLELINE*/,	// ���� ����(�ɼ�)
		D3DCOLOR_ARGB(255, 255, 255, 255));

}
