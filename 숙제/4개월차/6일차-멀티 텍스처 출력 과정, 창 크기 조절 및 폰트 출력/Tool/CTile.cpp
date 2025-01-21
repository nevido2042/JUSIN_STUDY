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

	_pDevice->Get_Sprite()->Draw(m_pTexInfo->pTexture, //출력할 텍스처 컴객체
		nullptr,		// 출력할 이미지 영역에 대한 Rect 주소, null인 경우 이미지의 0, 0기준으로 출력
		&vTemp,		// 출력할 이미지의 중심 좌표 vec3 주소, null인 경우 0, 0 이미지 중심
		&m_tInfo.vPos,		// 위치 좌표에 대한 vec3 주소, null인 경우 스크린 상 0, 0 좌표 출력	
		D3DCOLOR_ARGB(255, 255, 255, 255)); // 출력할 이미지와 섞을 색상 값, 0xffffffff를 넘겨주면 섞지 않고 원본 색상 유지

	// 폰트의 출력 위치를 조정 가능
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
		szBuf,											// 출력할 문자열
		lstrlen(szBuf),									// 문자열 버퍼의 크기
		&tRect,											// 출력할 렉트 위치
		DT_CENTER | DT_VCENTER /*| DT_SINGLELINE*/,	// 정렬 기준(옵션)
		D3DCOLOR_ARGB(255, 255, 255, 255));

}
