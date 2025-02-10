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
        nullptr, // ����� �̹��� ������ RECT �ּ�, NULL�� ��� 0, 0 �������� ���
        &vTemp, // ����� �̹����� �߽� ��ǥ, VEC3�ּ�, NULL �� ��� 0, 0�� �߽� ��ǥ
        nullptr, // �ؽ�ó�� ����� ��ġ ��ǥ, VEC3�ּ�, ��ũ���� 0, 0 ��ǥ�� ���
        D3DCOLOR_ARGB(255, 255, 255, 255)); // ����� ���� �̹����� ���� ����, 0xffffffff�� �Ѱ��ָ� ������ ����
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
