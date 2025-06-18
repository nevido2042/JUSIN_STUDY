#include "Shell.h"

#include "GameInstance.h"

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
	//m_vPreviousPos = m_vFirePos;
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

	//�� ���� ������ ���ֶ�
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
		//���� x, z �� �ͷ����� ���̸� �޾ƿͼ�
		//�ͷ����� ���̺��� �������� �� ���� ���� �İ�
		//���������
		_float vTerrainHeight = XMVectorGetY(m_pTargetBuffer->Compute_HeightPosition(m_pTransformCom->Get_State(STATE::POSITION)));
		_float vShellHeight = XMVectorGetY(m_pTransformCom->Get_State(STATE::POSITION));

		if (vTerrainHeight > vShellHeight)
		{
			_float3 vDigCenter;
			XMStoreFloat3(&vDigCenter, m_pTransformCom->Get_State(STATE::POSITION));
			static_cast<CVIBuffer_Terrain*>(m_pTargetBuffer)->DigGround(vDigCenter, SHELL_DIG_RADIUS, SHELL_DIG_DEPTH);
			Destroy();

			POSITION_DESC PosDesc = {};
			//���� �Ŀ��ٴ� ��Ŷ�� ������
			PosDesc.iID = m_iID;
			PosDesc.vPos = vDigCenter;
			m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_DIG), &PosDesc);
		}
	}
}

void CShell::Late_Update(_float fTimeDelta)
{
	if (m_pGameInstance->Is_In_Frustum(m_pTransformCom->Get_State(STATE::POSITION), 0.1f))
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);

	XMStoreFloat3(&m_vPreviousPos, m_pTransformCom->Get_State(STATE::POSITION));

}

HRESULT CShell::Render()
{
#ifdef _DEBUG
	m_pColliderCom->Render();
#endif
	return S_OK;
}

void CShell::Check_RaycastHit()
{
	CGameObject* pHitResult = { nullptr };
	_float fMinDist = FLT_MAX;

	_float fDist = { };
	_vector vOrigin = XMVectorSetW(XMLoadFloat3(&m_vPreviousPos), 1.f);
	_vector vRayDir = m_pTransformCom->Get_State(STATE::POSITION) - vOrigin;
	_float fRayLength = XMVectorGetX(XMVector3Length(vRayDir));
	vRayDir = XMVector3Normalize(vRayDir);
	CGameObject* pHit = { nullptr };

	pHit = m_pGameInstance->Check_RaycastHit(ENUM_CLASS(COLLISION_GROUP::MODULE), TEXT("Com_Collider"), vOrigin, vRayDir, fDist, m_iID);
	if (pHit)
	{
		if (fRayLength > fDist)
		{
			if (fDist < fMinDist)
			{
				fMinDist = fDist;
				pHitResult = pHit;
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
			}
		}
	}

	if (pHitResult != nullptr)
	{
		//����̶�� �������� �ְ�

		pHitResult->On_RaycastHit(this);

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
	Safe_Release(m_pShaderCom);
}
