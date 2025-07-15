#include "Shell.h"

#include "GameInstance.h"
#include "Module.h"
#include "Tank.h"
#include "DamagePanel.h"
#include "VIBuffer_Trail.h"

#include "TotalDamage.h"
#include "CountDamageModule.h"
#include "ShellDecal.h"

CShell::CShell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CShell::CShell(const CShell& Prototype)
	: CGameObject(Prototype)
{

}

HRESULT CShell::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CShell::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	SHELL_DESC* pDesc = static_cast<SHELL_DESC*>(pArg);

	m_pTargetBuffer = static_cast<CVIBuffer*>(m_pGameInstance->Get_Component(pDesc->iLevelIndex, pDesc->strLayerTag, pDesc->strComponentTag, pDesc->iIndex));
	Safe_AddRef(m_pTargetBuffer);
	if (nullptr == m_pTargetBuffer)
		return E_FAIL;

	m_vFirePos = pDesc->vFirePos;

	XMStoreFloat3(&m_vPreviousPos, m_pTransformCom->Get_State(STATE::POSITION));

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}


void CShell::Priority_Update(_float fTimeDelta)
{
	if(m_pGameInstance->Get_ID() == m_iID)
		m_pGameInstance->Add_CollisionGroup(ENUM_CLASS(COLLISION_GROUP::SHELL), this, TEXT("Com_Collider"));
}

void CShell::Update(_float fTimeDelta)
{
	m_pTransformCom->Apply_Gravity(9.8f, fTimeDelta);
	m_pTransformCom->Move_Velocity(fTimeDelta);

	m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));

	//맵 밖을 나가면 없애라
	_float3 vPos{};
	XMStoreFloat3(&vPos, m_pTransformCom->Get_State(STATE::POSITION));

	if (vPos.y < 50.f ||
		vPos.x < 1.f || vPos.x > TERRAIN_SIZE * TERRAIN_OFFSET_WIDTH - 1.f ||
		vPos.z < 1.f || vPos.z > TERRAIN_SIZE * TERRAIN_OFFSET_WIDTH - 1.f)
		Destroy();

	if (m_bisDestroyed == true)
		return;

#pragma region HIT
	if (m_pGameInstance->Get_ID() == m_iID || m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::PRACTICE))
	{
		Check_RaycastHit();
	}

#pragma endregion

	if (m_pGameInstance->Get_ID() == m_iID)
	{
		//현재 x, z 로 터레인의 높이를 받아와서
		//터레인의 높이보다 낮아지면 그 곳의 땅을 파고
		//사라져야함
		_float vTerrainHeight = XMVectorGetY(m_pTargetBuffer->Compute_HeightPosition(m_pTransformCom->Get_State(STATE::POSITION)));
		_float vShellHeight = XMVectorGetY(m_pTransformCom->Get_State(STATE::POSITION));

		if (vTerrainHeight > vShellHeight)
		{
			_float3 vDigCenter;
			XMStoreFloat3(&vDigCenter, m_pTransformCom->Get_State(STATE::POSITION));
			static_cast<CVIBuffer_Terrain*>(m_pTargetBuffer)->DigGround(vDigCenter, SHELL_DIG_RADIUS, SHELL_DIG_DEPTH);
			Destroy();

			POSITION_DESC PosDesc = {};
			//땅이 파였다는 패킷을 보내라
			PosDesc.iID = m_iID;
			PosDesc.vPos = vDigCenter;
			m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_DIG), &PosDesc);

			GAMEOBJECT_DESC Desc = {};
			Desc.vInitPosition = vDigCenter;
			m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_HitSmoke"), m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_HitSmoke"), &Desc);

			CShellDecal::SHELLDECAL_DESC ShellDecalDesc = {};
			ShellDecalDesc.vInitPosition = vDigCenter;
			ShellDecalDesc.vInitPosition.y -= SHELL_DIG_DEPTH * 0.5f;
			//ShellDecalDesc.vFirePos = m_vFirePos;
			ShellDecalDesc.bIsGround = true;
			m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_ShellDecal"), m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_ShellDecal"), &ShellDecalDesc);

		}
	}


#pragma region 트레일 업데이트

	m_pVIBufferCom->UpdateTrailBuffer(XMVectorSetW(XMLoadFloat3(&m_vPreviousPos), 1.f), m_pTransformCom->Get_State(STATE::POSITION), m_pTransformCom->Get_WorldMatrix_Inverse(), 0.15f);

#pragma endregion

}

void CShell::Late_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(STATE::POSITION), 0.1f))
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONLIGHT, this);

	XMStoreFloat3(&m_vPreviousPos, m_pTransformCom->Get_State(STATE::POSITION));

}

HRESULT CShell::Render()
{

	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	//if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
	//	return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(6)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

#ifdef _DEBUG
	//m_pColliderCom->Render();
#endif
	return S_OK;
}

void CShell::Check_RaycastHit()
{
	COLLISION_GROUP eCollGroup = { COLLISION_GROUP::END };
	CGameObject* pHitResult = { nullptr };
	_float fMinDist = FLT_MAX;

	_float fDist = { };
	_vector vOrigin = XMVectorSetW(XMLoadFloat3(&m_vPreviousPos), 1.f);
	_vector vRayDir = m_pTransformCom->Get_State(STATE::POSITION) - vOrigin;
	_float fRayLength = XMVectorGetX(XMVector3Length(vRayDir));
	vRayDir = XMVector3Normalize(vRayDir);
	CGameObject* pHit = { nullptr };

	//차체나 포탑을 맞았을 때, 부품 충돌을 검사하라
	pHit = m_pGameInstance->Check_RaycastHit(ENUM_CLASS(COLLISION_GROUP::BODY), TEXT("Com_Collider"), vOrigin, vRayDir, fDist, m_iID);
	if (pHit)
	{
		if (fRayLength > fDist)
		{
			if (fDist < fMinDist)
			{
				fMinDist = fDist;
				pHitResult = pHit;
				eCollGroup = COLLISION_GROUP::BODY;
			}
		}
	}

	pHit = m_pGameInstance->Check_RaycastHit(ENUM_CLASS(COLLISION_GROUP::TURRET), TEXT("Com_Collider"), vOrigin, vRayDir, fDist, m_iID);
	if (pHit)
	{
		if (fRayLength > fDist)
		{
			if (fDist < fMinDist)
			{
				fMinDist = fDist;
				pHitResult = pHit;
				eCollGroup = COLLISION_GROUP::TURRET;
			}
		}
	}

	pHit = m_pGameInstance->Check_RaycastHit(ENUM_CLASS(COLLISION_GROUP::BUILDING), TEXT("Com_Collider"), vOrigin, vRayDir, fDist);
	if (pHit)
	{
		if (fRayLength > fDist)
		{
			if (fDist < fMinDist)
			{
				fMinDist = fDist;
				pHitResult = pHit;
				eCollGroup = COLLISION_GROUP::BUILDING;
			}
		}
	}

	if (pHitResult != nullptr)
	{

		_float3 vHitPos = {};
		XMStoreFloat3(&vHitPos, vOrigin + vRayDir * fMinDist);

		GAMEOBJECT_DESC Desc = {};
		Desc.vInitPosition = vHitPos;
		m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_HitSmoke"), m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_HitSmoke"), &Desc);

		CShellDecal::SHELLDECAL_DESC ShellDecalDesc = {};
		ShellDecalDesc.vInitPosition = vHitPos;
		ShellDecalDesc.vFirePos = m_vFirePos;
		m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_ShellDecal"), m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_ShellDecal"), &ShellDecalDesc);

		if (eCollGroup == COLLISION_GROUP::TURRET || eCollGroup == COLLISION_GROUP::BODY)
		{
			GAMEOBJECT_DESC Desc = {};
			Desc.vInitPosition = vHitPos;
			m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Spark"), m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Spark"), &Desc);
		}


		//포탑이나 맞았을 때 부품 검사
		if (eCollGroup == COLLISION_GROUP::TURRET)
		{
			CModule* pTurret = static_cast<CModule*>(pHitResult);
			//죽었으면 무시
			if (pTurret->Get_Owner()->Get_isDie())
				return;

			pTurret->TakeDamage_Onwer(10.f, this);
			pTurret->On_RaycastHit(this);

			//딜량 UI에 누적해라
			CTotalDamage* pTotalDamage = static_cast<CTotalDamage*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_TotalDamage")));
			if (pTotalDamage)
			{
				pTotalDamage->AddDamage(10.f);
			}

			//데미지 로그를 띄워라
			CUIObject::UIOBJECT_DESC				DescDamageLog{};
			DescDamageLog.fSizeX = 400.f * 2.f * UI_RATIO;
			DescDamageLog.fSizeY = 38.f * 2.f * UI_RATIO;
			DescDamageLog.fY = g_iWinSizeY * 0.8f;
			DescDamageLog.fX = g_iWinSizeX * 0.5f;

			m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DamageLog"),
				m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_DamageLog"), &DescDamageLog);

			//부품 부순 횟수 증가
			CCountDamageModule* pCountDamageModule = static_cast<CCountDamageModule*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_CountDamageModule")));
			if (pCountDamageModule)
			{
				pCountDamageModule->AddCountDamageModule();
			}

			//장비 데미지 로그를 띄워라
			CUIObject::UIOBJECT_DESC				DMGLogModuleDesc{};
			DMGLogModuleDesc.fSizeX = 400.f * 2.f * UI_RATIO;
			DMGLogModuleDesc.fSizeY = 38.f * 2.f * UI_RATIO;
			DMGLogModuleDesc.fY = g_iWinSizeY * 0.75f;
			DMGLogModuleDesc.fX = g_iWinSizeX * 0.5f;

			m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DamageLogModule"),
				m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_DamageLogModule"), &DMGLogModuleDesc);

			//부품 충돌 검사를 해라
			CGameObject* pHitModule = { nullptr };
			pHitModule = m_pGameInstance->Check_RaycastHit(ENUM_CLASS(COLLISION_GROUP::MODULE), TEXT("Com_Collider"), vOrigin, vRayDir, fDist, m_iID);
			if (pHitModule)
			{
				pHitModule->On_RaycastHit(this);

				//부품 부순 횟수 증가
				pCountDamageModule->AddCountDamageModule();
			}

			if (m_pGameInstance->Get_ID() == m_iID)
			{
				CGameObject* pDamagePanel = m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_DamagePanel"));
				if (pDamagePanel)
				{
					static_cast<CDamagePanel*>(pDamagePanel)->Play_Voice_Hit_Enemy();
				}
			}
		}
		//차체를 맞았을 때 부품 검사
		else if (eCollGroup == COLLISION_GROUP::BODY)
		{
			CTank* pTank = static_cast<CTank*>(pHitResult);
			//죽었으면 무시
			if (pTank->Get_isDie())
				return;

			pTank->Take_Damage(10.f, this);

			//딜량 UI에 누적해라
			CTotalDamage* pTotalDamage = static_cast<CTotalDamage*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_TotalDamage")));
			if (pTotalDamage)
			{
				pTotalDamage->AddDamage(10.f);
			}

			//데미지 로그를 띄워라
			CUIObject::UIOBJECT_DESC				DescDamageLog{};
			DescDamageLog.fSizeX = 400.f * 2.f * UI_RATIO;
			DescDamageLog.fSizeY = 38.f * 2.f * UI_RATIO;
			DescDamageLog.fY = g_iWinSizeY * 0.8f;
			DescDamageLog.fX = g_iWinSizeX * 0.5f;

			m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DamageLog"),
				m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_DamageLog"), &DescDamageLog);

			//부품 충돌 검사를 해라
			CGameObject* pHitModule = { nullptr };
			pHitModule = m_pGameInstance->Check_RaycastHit(ENUM_CLASS(COLLISION_GROUP::MODULE), TEXT("Com_Collider"), vOrigin, vRayDir, fDist, m_iID);
			if (pHitModule)
			{
				pHitModule->On_RaycastHit(this);

				//부품 부순 횟수 증가
				CCountDamageModule* pCountDamageModule = static_cast<CCountDamageModule*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_CountDamageModule")));
				if (pCountDamageModule)
				{
					pCountDamageModule->AddCountDamageModule();

					//장비 데미지 로그를 띄워라
					CUIObject::UIOBJECT_DESC				DMGLogModuleDesc{};
					DMGLogModuleDesc.fSizeX = 400.f * 2.f * UI_RATIO;
					DMGLogModuleDesc.fSizeY = 38.f * 2.f * UI_RATIO;
					DMGLogModuleDesc.fY = g_iWinSizeY * 0.75f;
					DMGLogModuleDesc.fX = g_iWinSizeX * 0.5f;

					m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_DamageLogModule"),
						m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_DamageLogModule"), &DMGLogModuleDesc);
				}
			}

			if (m_pGameInstance->Get_ID() == m_iID)
			{
				CGameObject* pDamagePanel = m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_DamagePanel"));
				if (pDamagePanel)
				{
					static_cast<CDamagePanel*>(pDamagePanel)->Play_Voice_Hit_Enemy();
				}
			}
		}
		else if (eCollGroup == COLLISION_GROUP::BUILDING)
		{

		}

		Destroy();
	}
}


HRESULT CShell::Ready_Components()
{
	/* For.Com_Collider */
	CBounding_Sphere::SPHERE_DESC	SphereDesc{};
	SphereDesc.fRadius = 0.5f;
	SphereDesc.vCenter = _float3(0.f, 0.f, 0.f);
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &SphereDesc)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Trail"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CShell::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	return S_OK;
}

CShell* CShell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CShell* pInstance = new CShell(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CShell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CShell::Clone(void* pArg)
{
	CShell* pInstance = new CShell(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CShell");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShell::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
	Safe_Release(m_pTargetBuffer);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
}
