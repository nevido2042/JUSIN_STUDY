#include "Gun.h"

#include "GameInstance.h"

CGun::CGun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CGun::CGun(const CGun& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CGun::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CGun::Priority_Update(_float fTimeDelta)
{
	if (GetForegroundWindow() == g_hWnd)
	{
		if (m_pGameInstance->Get_ID() == m_iID && m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::GAMEPLAY))
		{
			if (m_pGameInstance->Key_Down(DIK_R))
			{
				BOOL_DESC Desc{};
				Desc.iID = m_iID;
				Desc.bBool = true;
				m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_UP), &Desc);
			}
			else if (m_pGameInstance->Key_Up(DIK_R))
			{
				BOOL_DESC Desc{};
				Desc.iID = m_iID;
				Desc.bBool = false;
				m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_UP), &Desc);
			}

			if (m_pGameInstance->Key_Down(DIK_F))
			{
				BOOL_DESC Desc{};
				Desc.iID = m_iID;
				Desc.bBool = true;
				m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_DOWN), &Desc);
			}
			else if (m_pGameInstance->Key_Up(DIK_F))
			{
				BOOL_DESC Desc{};
				Desc.iID = m_iID;
				Desc.bBool = false;
				m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_DOWN), &Desc);
			}

		}


		if (m_iID == m_pGameInstance->Get_ID() || m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::PRACTICE))
		{
			if (m_pGameInstance->Key_Pressing(DIK_R))
			{
				m_pTransformCom->Turn(XMVectorSet(1.f, 0.f, 0.f, 0.f), -fTimeDelta);
			}
			else if (m_pGameInstance->Key_Pressing(DIK_F))
			{
				m_pTransformCom->Turn(XMVectorSet(1.f, 0.f, 0.f, 0.f), fTimeDelta);
			}
		}
	}

	if (m_bUp)
		m_pTransformCom->Turn(XMVectorSet(1.f, 0.f, 0.f, 0.f), -fTimeDelta);
	else if (m_bDown)
		m_pTransformCom->Turn(XMVectorSet(1.f, 0.f, 0.f, 0.f), fTimeDelta);
}

void CGun::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_ID() == m_iID && GetForegroundWindow() == g_hWnd && m_pGameInstance->Get_NewLevel_Index() != ENUM_CLASS(LEVEL::PRACTICE))
	{
		m_fTimeAcc += fTimeDelta;
		if (m_fTimeAcc > m_fSyncInterval)
		{
			m_fTimeAcc = 0.f;

			MATRIX_DESC Desc{};
			Desc.iID = m_iID;
			XMStoreFloat4x4(&Desc.Matrix, m_pTransformCom->Get_WorldMatrix());

			m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_MATRIX_GUN), &Desc);
		}
	}

	//부모의 월드 행렬을 가져와서 자신의 월드 행렬과 곱해준다.
	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixMultiply(m_pTransformCom->Get_WorldMatrix(), XMLoadFloat4x4(m_pParentWorldMatrix)));
}

void CGun::Free()
{
	__super::Free();

}
