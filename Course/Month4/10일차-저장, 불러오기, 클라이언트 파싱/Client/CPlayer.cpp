#include "pch.h"
#include "CPlayer.h"
#include "CTextureMgr.h"
#include "CDevice.h"
#include "TimeMgr.h"

CPlayer::CPlayer()
    :m_fWalkSpeed(0.f), m_eState(PLAYER_END), m_pStateKey(nullptr)
{
    ZeroMemory(&m_tFrame, sizeof(FRAME));
    ZeroMemory(&m_vClickPos, sizeof(D3DXVECTOR3));
}

CPlayer::~CPlayer()
{
    Release();
}

HRESULT CPlayer::Initialize(void)
{
    Set_State(PLAYER_IDLE);

    m_fWalkSpeed = TILECX + TILECY;

    return E_NOTIMPL;
}

int CPlayer::Update(void)
{
    //애니메이션
    m_tFrame.fFrame += CTimeMgr::Get_Instance()->Get_TimeDelta() * 10.f;

    if (m_tFrame.fFrame > m_tFrame.fMax)
    {
        m_tFrame.fFrame = 0.f;
    }

    //마우스 클릭
    if (GetAsyncKeyState(VK_LBUTTON))
    {
        m_vClickPos = Get_Mouse() - m_vScroll;
    }

    //클릭한 위치랑 현재 위치가 다르면 이동하라
    D3DXVECTOR3 vDif = m_vClickPos - m_tInfo.vPos;
    float fDistanceSq = D3DXVec3LengthSq(&vDif);
    if (fDistanceSq > 0.1f)//허용 오차 0.1f
    {
        Set_State(PLAYER_WALK);
    }
    else
    {
        Set_State(PLAYER_IDLE);
    }

    if (m_eState == PLAYER_WALK)
    {
        //노말벡터 구하기
        D3DXVECTOR3 vDir = m_vClickPos - m_tInfo.vPos;
        D3DXVec3Normalize(&vDir, &vDir);

        m_tInfo.vPos += vDir * m_fWalkSpeed * CTimeMgr::Get_Instance()->Get_TimeDelta();
    }

    return 0;
}

void CPlayer::Late_Update(void)
{
}

void CPlayer::Render(void)
{
    //월드 행렬
    D3DXMATRIX		matWorld, matScale, matTrans;
    D3DXMatrixIdentity(&matWorld);

    if (m_eState == PLAYER_WALK &&
        m_vClickPos.x < m_tInfo.vPos.x)
    {
        D3DXMatrixScaling(&matScale, -1.f, 1.f, 1.f);
    }
    else
    {
        D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
    }
    
    D3DXMatrixTranslation(&matTrans,
        m_tInfo.vPos.x + m_vScroll.x,
        m_tInfo.vPos.y + m_vScroll.y,
        0.f);
    matWorld = matScale * matTrans;
    CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

    //이미지 출력
    const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Player", m_pStateKey, (int)m_tFrame.fFrame);
    D3DXVECTOR3 vTemp{ pTexInfo->tImgInfo.Width * 0.5f,pTexInfo->tImgInfo.Height * 0.5f, 0.f };
    CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
        nullptr, // 출력할 이미지 영역의 RECT 주소, NULL인 경우 0, 0 기준으로 출력
        &vTemp, // 출력할 이미지의 중심 좌표, VEC3주소, NULL 인 경우 0, 0이 중심 좌표
        nullptr, // 텍스처를 출력할 위치 좌표, VEC3주소, 스크린상 0, 0 좌표에 출력
        D3DCOLOR_ARGB(255, 255, 255, 255)); // 출력할 원본 이미지와 섞을 색상, 0xffffffff를 넘겨주면 원본색 유지

    //디버깅
    TCHAR szBuf[MAX_STR] = L"";
    switch (m_eState)
    {
    case PLAYER_IDLE:
        swprintf_s(szBuf, L"IDLE");
        break;
    case PLAYER_WALK:
        swprintf_s(szBuf, L"WALK");
        break;
    case PLAYER_END:
        break;
    default:
        break;
    }

    CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(), szBuf, lstrlen(szBuf), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));


}

void CPlayer::Release(void)
{
}

void CPlayer::Set_State(PLAYER_STATE _eState)
{
    if (m_eState != _eState)
    {
        m_eState = _eState;
    }
    else
    {
        return;
    }

    m_tFrame.fFrame = 0.f;

    switch (m_eState)
    {
    case PLAYER_IDLE:
        m_tFrame.fMax = 12.f;
        m_pStateKey = L"IDLE";
        break;
    case PLAYER_WALK:
        m_tFrame.fMax = 13.f;
        m_pStateKey = L"WALK";
        break;
    case PLAYER_END:
        break;
    default:
        break;
    }
}
