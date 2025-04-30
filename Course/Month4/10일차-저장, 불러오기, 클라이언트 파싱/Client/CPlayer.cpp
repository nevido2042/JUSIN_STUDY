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
    //�ִϸ��̼�
    m_tFrame.fFrame += CTimeMgr::Get_Instance()->Get_TimeDelta() * 10.f;

    if (m_tFrame.fFrame > m_tFrame.fMax)
    {
        m_tFrame.fFrame = 0.f;
    }

    //���콺 Ŭ��
    if (GetAsyncKeyState(VK_LBUTTON))
    {
        m_vClickPos = Get_Mouse() - m_vScroll;
    }

    //Ŭ���� ��ġ�� ���� ��ġ�� �ٸ��� �̵��϶�
    D3DXVECTOR3 vDif = m_vClickPos - m_tInfo.vPos;
    float fDistanceSq = D3DXVec3LengthSq(&vDif);
    if (fDistanceSq > 0.1f)//��� ���� 0.1f
    {
        Set_State(PLAYER_WALK);
    }
    else
    {
        Set_State(PLAYER_IDLE);
    }

    if (m_eState == PLAYER_WALK)
    {
        //�븻���� ���ϱ�
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
    //���� ���
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

    //�̹��� ���
    const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Player", m_pStateKey, (int)m_tFrame.fFrame);
    D3DXVECTOR3 vTemp{ pTexInfo->tImgInfo.Width * 0.5f,pTexInfo->tImgInfo.Height * 0.5f, 0.f };
    CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
        nullptr, // ����� �̹��� ������ RECT �ּ�, NULL�� ��� 0, 0 �������� ���
        &vTemp, // ����� �̹����� �߽� ��ǥ, VEC3�ּ�, NULL �� ��� 0, 0�� �߽� ��ǥ
        nullptr, // �ؽ�ó�� ����� ��ġ ��ǥ, VEC3�ּ�, ��ũ���� 0, 0 ��ǥ�� ���
        D3DCOLOR_ARGB(255, 255, 255, 255)); // ����� ���� �̹����� ���� ����, 0xffffffff�� �Ѱ��ָ� ������ ����

    //�����
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
