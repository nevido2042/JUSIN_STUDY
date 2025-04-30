#include "pch.h"
#include "CPlayer.h"
#include "CDevice.h"
#include "CTextureMgr.h"
#include "CAstarMgr.h"
#include "TimeMgr.h"

CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
    Release();
}

HRESULT CPlayer::Initialize(void)
{
    m_tInfo.vPos = { 100.f, 100.f, 0.f };
    m_wstrObjKey = L"Player";
    m_wstrStateKey = L"Dash";

    return S_OK;
}

int CPlayer::Update(void)
{
    D3DXMATRIX		matScale, matTrans;

    D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
    D3DXMatrixTranslation(&matTrans,
        m_tInfo.vPos.x + m_vScroll.x,
        m_tInfo.vPos.y + m_vScroll.y,
        0.f);

    m_tInfo.matWorld = matScale * matTrans;

    return 0;
}

void CPlayer::Late_Update(void)
{
    if (GetAsyncKeyState(VK_LBUTTON))
    {
        CAstarMgr::Get_Instance()->Start_Astar(m_tInfo.vPos, Get_Mouse() - CObj::m_vScroll);
    }

    Move_Astar();
}

void CPlayer::Render(void)
{
    CDevice::Get_Instance()->Get_Sprite()->SetTransform(&m_tInfo.matWorld);

    const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(m_wstrObjKey.c_str(), m_wstrStateKey.c_str(),        0);

    float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
    float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;

    D3DXVECTOR3	vTemp = { fCenterX, fCenterY, 0.f };

    CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
        nullptr, // 출력할 이미지 영역의 RECT 주소, NULL인 경우 0, 0 기준으로 출력
        &vTemp, // 출력할 이미지의 중심 좌표, VEC3주소, NULL 인 경우 0, 0이 중심 좌표
        nullptr, // 텍스처를 출력할 위치 좌표, VEC3주소, 스크린상 0, 0 좌표에 출력
        D3DCOLOR_ARGB(255, 255, 255, 255)); // 출력할 원본 이미지와 섞을 색상, 0xffffffff를 넘겨주면 원본색 유지
}

void CPlayer::Release(void)
{
}

void CPlayer::Move_Astar()
{
    list<TILE*>& BestList = CAstarMgr::Get_Instance()->Get_BestList();

    if (!BestList.empty())
    {
        D3DXVECTOR3 vDir = BestList.front()->vPos - m_tInfo.vPos;

        float       fDistance = D3DXVec3Length(&vDir);

        D3DXVec3Normalize(&vDir, &vDir);

        m_tInfo.vPos += vDir * 300.f * CTimeMgr::Get_Instance()->Get_TimeDelta();

        if (3.f >= fDistance)
            BestList.pop_front();
    }
}
