#include "Turret.h"

#include "GameInstance.h"

CTurret::CTurret(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CTurret::CTurret(const CTurret& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CTurret::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CTurret::Priority_Update(_float fTimeDelta)
{
	if (GetForegroundWindow() == g_hWnd)
	{
		if (m_pGameInstance->Get_ID() == m_iID && m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::GAMEPLAY))
		{
			if (m_pGameInstance->Key_Down(DIK_Q))
			{
				BOOL_DESC Desc{};
				Desc.iID = m_iID;
				Desc.bBool = true;
				m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_LEFT), &Desc);
			}
			else if (m_pGameInstance->Key_Up(DIK_Q))
			{
				BOOL_DESC Desc{};
				Desc.iID = m_iID;
				Desc.bBool = false;
				m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_LEFT), &Desc);
			}

			if (m_pGameInstance->Key_Down(DIK_E))
			{
				BOOL_DESC Desc{};
				Desc.iID = m_iID;
				Desc.bBool = true;
				m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_RIGHT), &Desc);
			}
			else if (m_pGameInstance->Key_Up(DIK_E))
			{
				BOOL_DESC Desc{};
				Desc.iID = m_iID;
				Desc.bBool = false;
				m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_RIGHT), &Desc);
			}

		}


		if (m_iID == m_pGameInstance->Get_ID() || m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::PRACTICE))
		{
			if (m_pGameInstance->Key_Pressing(DIK_Q))
			{
				m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -fTimeDelta);
			}
			else if (m_pGameInstance->Key_Pressing(DIK_E))
			{
				m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta);
			}
		}
	}

	if (m_bLeft)
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -fTimeDelta);
	else if (m_bRight)
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta);


	CGameObject::Priority_Update(fTimeDelta);
}

void CTurret::Update(_float fTimeDelta)
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

			m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_MATRIX_TURRET), &Desc);
		}
	}

	//부모의 월드 행렬을 가져와서 자신의 월드 행렬과 곱해준다.
	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixMultiply(m_pTransformCom->Get_WorldMatrix(), XMLoadFloat4x4(m_pParentWorldMatrix)));

	CGameObject::Update(fTimeDelta);

}



void CTurret::Free()
{
	__super::Free();

}
