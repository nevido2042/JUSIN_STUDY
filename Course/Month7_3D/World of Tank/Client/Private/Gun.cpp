#include "Gun.h"

#include "GameInstance.h"
//#include "Terrain.h"
#include "GameManager.h"
#include "PickedManager.h"
#include "Shell.h"

CGun::CGun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CModule(pDevice, pContext)
{
}

CGun::CGun(const CGun& Prototype)
	:CModule(Prototype)
{
}

HRESULT CGun::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	GUN_DESC* pGunDesc = static_cast<GUN_DESC*>(pArg);
	m_vBaseColor = pGunDesc->vBaseColor;

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
			m_pTransformCom->Turn(vAxis, -fTimeDelta * m_fRotateSpeed);
		// 하향 회전 시 제한
		else if (m_bDown && fDotY >= m_fMinPitch)
			m_pTransformCom->Turn(vAxis, fTimeDelta * m_fRotateSpeed);
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

HRESULT CGun:: Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (m_pModelCom)
	{
		_uint		iNumMesh = m_pModelCom->Get_NumMeshes();

		for (_uint i = 0; i < iNumMesh; i++)
		{
			if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE, 0)))
				return E_FAIL;

			if (FAILED(m_pShaderCom->Begin(2)))
				return E_FAIL;

			if (FAILED(m_pModelCom->Render(i)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CGun::Fire()
{
	CShell::SHELL_DESC Desc = {};
	memcpy(&Desc.vInitPosition, m_CombinedWorldMatrix.m[3], sizeof(_float3));
	memcpy(&Desc.vVelocity, m_CombinedWorldMatrix.m[2], sizeof(_float3));
	_vector vVelocity = XMLoadFloat3(&Desc.vVelocity);
	vVelocity = XMVectorScale(vVelocity, 100.f);
	XMStoreFloat3(&Desc.vVelocity, vVelocity);
	Desc.iID = m_iID;

	Desc.iLevelIndex = m_pGameInstance->Get_NewLevel_Index();
	Desc.strLayerTag = TEXT("Layer_Terrain");
	Desc.iIndex = 0;
	Desc.strComponentTag = TEXT("Com_VIBuffer");
	memcpy(&Desc.vFirePos, m_CombinedWorldMatrix.m[3], sizeof(_float3));

	m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Shell"), m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Shell"), &Desc);

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
		//CTerrain* pTerrain = static_cast<CTerrain*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Terrain")));
		//if (pTerrain == nullptr)
		//	return;

		// 내 위치
		_float3 vMyPos = {
			m_CombinedWorldMatrix.m[3][0],
			m_CombinedWorldMatrix.m[3][1],
			m_CombinedWorldMatrix.m[3][2]
		};

		// Pick된 위치
		_float3 vPicked = {};

		CPickedManager* pPickedManager = static_cast<CPickedManager*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_PickedManager")));
		if (pPickedManager)
			vPicked = pPickedManager->Get_ScreenCenterPickedPos();

		// 방향 벡터 (월드 공간)
		_vector vToPicked = XMLoadFloat3(&vPicked) - XMLoadFloat3(&vMyPos);

		// 내 Up, Look, Right 추출
		_vector vUp = XMVector3Normalize(XMLoadFloat3((_float3*)&m_CombinedWorldMatrix.m[1]));
		_vector vLook = XMVector3Normalize(XMLoadFloat3((_float3*)&m_CombinedWorldMatrix.m[2]));
		_vector vRight = XMVector3Normalize(XMLoadFloat3((_float3*)&m_CombinedWorldMatrix.m[0]));

		// Look-Up 평면에 투영 = Right 축 제거
		_vector vProjected = vToPicked - XMVector3Dot(vToPicked, vRight) * vRight;  // vToPicked의 Right 성분 제거

		vProjected = XMVector3Normalize(vProjected);

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

		if (fUpDot > 0.01f) //하로
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
				m_pTransformCom->Turn(vAxis, fTimeDelta * m_fRotateSpeed/** abs(fUpDot)*/);
		}
		else if (fUpDot < -0.01f) //상으로
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
				m_pTransformCom->Turn(vAxis, -fTimeDelta * m_fRotateSpeed /** abs(fUpDot)*/);
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

				// 회전이 멈췄을 때, 정확히 바라보도록 Look을 설정
				// 새로운 Look 벡터 설정
				// 회전 멈출 때 정확히 타겟을 바라보게 Look 보정
				_float3 vScaled = m_pTransformCom->Get_Scaled(); // 현재 스케일 유지
				// 정면 방향 (Look)
				_vector vLook = XMVector3Normalize(vProjected); // 목표 방향 정규화
				// 오른쪽 방향
				_vector vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook));
				// 위 방향
				_vector vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));
				// 스케일 적용
				vLook *= vScaled.z;
				vRight *= vScaled.x;
				vUp *= vScaled.y;
				// CombinedWorldMatrix에 직접 반영
				XMStoreFloat4(reinterpret_cast<_float4*>(&m_CombinedWorldMatrix.m[0]), vRight);
				XMStoreFloat4(reinterpret_cast<_float4*>(&m_CombinedWorldMatrix.m[1]), vUp);
				XMStoreFloat4(reinterpret_cast<_float4*>(&m_CombinedWorldMatrix.m[2]), vLook);
			}
		}
	}
}

HRESULT CGun::Bind_ShaderResources()
{
	//파트 오브젝트는 자기 트랜스폼 안써야한다
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vBaseColor", &m_vBaseColor, sizeof(_float4))))
		return E_FAIL;

	//격납고 레벨이라면 실시간으로 게임매니저에서, 색깔을 가져와서 바인딩한다.
	if (m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::HANGER))
	{
		CGameManager* pGameManager = GET_GAMEMANAGER;

		if (FAILED(m_pShaderCom->Bind_RawValue("g_vBaseColor", &pGameManager->Get_GunColor(), sizeof(_float4))))
			return E_FAIL;
	}

	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_Light(0);

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

void CGun::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pSoundCom);
}
