#include "Turret.h"

#include "GameInstance.h"
#include "Terrain.h"

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
	Input(fTimeDelta);

	if (m_pGameInstance->Get_ID() != m_iID && m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::GAMEPLAY))
	{
		if (m_bLeft)
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -fTimeDelta);
		else if (m_bRight)
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta);
	}

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

void CTurret::Input(_float fTimeDelta)
{
	if (GetForegroundWindow() != g_hWnd)
		return;

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

	// 타겟 위치
	CTerrain* pTerrain = static_cast<CTerrain*>(m_pGameInstance->Get_Last_GameObject(
		m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Terrain")));

	_float3 vTargetPos = pTerrain->Get_PickedPos();
	vTargetPos.y = m_CombinedWorldMatrix.m[3][1]; // 자기와 y만 맞추기

	// 내 위치
	_float3 vMyPos = {
		m_CombinedWorldMatrix.m[3][0],
		m_CombinedWorldMatrix.m[3][1],
		m_CombinedWorldMatrix.m[3][2]
	};

	// 방향 벡터
	_vector vToTarget = XMVector3Normalize(XMLoadFloat3(&vTargetPos) - XMLoadFloat3(&vMyPos));
	_vector vRight = XMLoadFloat4(reinterpret_cast<const _float4*>(&m_CombinedWorldMatrix.m[0]));

	// 좌/우 판별
	_float fRightDot = XMVectorGetX(XMVector3Dot(vRight, vToTarget));

	// 출력
	//cout << "Right Dot : " << fRightDot << endl;

	if (fRightDot > 0.01f)//왼쪽 회전
	{
		/*if (m_bLeft == false)
		{
			m_bLeft = true;
			m_bRight = false;

			BOOL_DESC Desc{};
			Desc.iID = m_iID;
			Desc.bBool = true;
			m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_LEFT), &Desc);
			Desc.bBool = false;
			m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_RIGHT), &Desc);

		}*/

		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta);
	}
	else if (fRightDot < -0.01f)//오른 쪽 회전
	{
		/*if (m_bRight == false)
		{
			m_bLeft = false;
			m_bRight = true;

			BOOL_DESC Desc{};
			Desc.iID = m_iID;
			Desc.bBool = false;
			m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_LEFT), &Desc);
			Desc.bBool = true;
			m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_RIGHT), &Desc);
		}*/

		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), -fTimeDelta);
	}
}



void CTurret::Free()
{
	__super::Free();

}
