#include "Gun.h"

#include "GameInstance.h"
#include "Terrain.h"

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
	Input(fTimeDelta);

	if (m_pGameInstance->Get_ID() != m_iID && m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::GAMEPLAY))
	{
		// 회전 축 (x축)
		_vector vAxis = XMVectorSet(1.f, 0.f, 0.f, 0.f);

		// 현재 Look 벡터 가져오기
		_vector vLook = m_pTransformCom->Get_State(STATE::LOOK);
		vLook = XMVector3Normalize(vLook);

		// Pitch = Look 벡터와 세계 기준 (XZ 평면)의 각도
		// 여기선 Y값을 기준으로 판단 가능
		_float fDotY = XMVectorGetY(vLook); // = sin(pitch)

		// 상향 회전 시 제한
		if (m_bUp && fDotY <= m_fMaxPitch)
			m_pTransformCom->Turn(vAxis, -fTimeDelta);
		// 하향 회전 시 제한
		else if (m_bDown && fDotY >= m_fMinPitch)
			m_pTransformCom->Turn(vAxis, fTimeDelta);
	}
	
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

HRESULT CGun::Fire()
{
	_vector vPos = XMVectorSet(m_CombinedWorldMatrix._41, m_CombinedWorldMatrix._42, m_CombinedWorldMatrix._43, 1.0f);
	m_pSoundCom->Update3DPosition(vPos);

	m_pSoundCom->Play("wpn_1");

	return S_OK;
}

void CGun::Input(_float fTimeDelta)
{
	if (GetForegroundWindow() != g_hWnd)
		return;

	if (m_pGameInstance->Mouse_Pressing(ENUM_CLASS(DIMK::RBUTTON)))
		return;

	if (m_pGameInstance->Get_ID() == m_iID)
	{
		// 타겟 위치
		CTerrain* pTerrain = static_cast<CTerrain*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Terrain")));
		if (pTerrain == nullptr)
			return;

		// 내 위치
		_float3 vMyPos = {
			m_CombinedWorldMatrix.m[3][0],
			m_CombinedWorldMatrix.m[3][1],
			m_CombinedWorldMatrix.m[3][2]
		};

		// Pick된 위치
		_float3 vPicked = pTerrain->Get_PickedPos();

		// 방향 벡터 (월드 공간)
		_vector vToPicked = XMLoadFloat3(&vPicked) - XMLoadFloat3(&vMyPos);

		// 내 Up, Look, Right 추출 (정규화돼 있다고 가정)
		_vector vUp = XMVector3Normalize(XMLoadFloat3((_float3*)&m_CombinedWorldMatrix.m[1]));
		_vector vLook = XMVector3Normalize(XMLoadFloat3((_float3*)&m_CombinedWorldMatrix.m[2]));
		_vector vRight = XMVector3Normalize(XMLoadFloat3((_float3*)&m_CombinedWorldMatrix.m[0]));

		// Look-Up 평면에 투영 = Right 축 제거
		_vector vProjected = vToPicked - XMVector3Dot(vToPicked, vRight) * vRight;  // vToPicked의 Right 성분 제거

		vProjected = XMVector3Normalize(vProjected);  // 방향만 필요하다면 정규화

		// Up 벡터와 비교해 위/아래 판단
		_float fUpDot = XMVectorGetX(XMVector3Dot(vProjected, -vUp));

		// 회전 축 (x축)
		_vector vAxis = XMVectorSet(1.f, 0.f, 0.f, 0.f);

		// 현재 Look 벡터 가져오기
		_vector vLocalLook = m_pTransformCom->Get_State(STATE::LOOK);
		vLocalLook = XMVector3Normalize(vLocalLook);

		// Pitch = Look 벡터와 세계 기준 (XZ 평면)의 각도
		// 여기선 Y값을 기준으로 판단 가능
		_float fDotY = XMVectorGetY(vLocalLook); // = sin(pitch)

		if (fUpDot > 0.1f) //하로
		{
			if (m_bDown == false)
			{
				m_bDown = true;
				m_bUp = false;

				if (m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::GAMEPLAY))
				{
					BOOL_DESC Desc{};
					Desc.iID = m_iID;
					Desc.bBool = true;
					m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_DOWN), &Desc);
				}
			}
			if(fDotY >= m_fMinPitch)
				m_pTransformCom->Turn(vAxis, fTimeDelta);
		}
		else if (fUpDot < -0.1f) //상으로
		{
			if (m_bUp == false)
			{
				m_bUp = true;
				m_bDown = false;

				if (m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::GAMEPLAY))
				{
					BOOL_DESC Desc{};
					Desc.iID = m_iID;
					Desc.bBool = true;
					m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_UP), &Desc);
				}

			}
			if (fDotY <= m_fMaxPitch)
				m_pTransformCom->Turn(vAxis, -fTimeDelta);
		}
		else
		{
			if (m_bUp || m_bDown)
			{
				m_bUp = m_bDown = false;

				if (m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::GAMEPLAY))
				{
					BOOL_DESC Desc{};
					Desc.iID = m_iID;
					Desc.bBool = false;
					m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_UP), &Desc);
					Desc.bBool = false;
					m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_DOWN), &Desc);
				}

			}
		}
	}
}

void CGun::Free()
{
	__super::Free();

}
