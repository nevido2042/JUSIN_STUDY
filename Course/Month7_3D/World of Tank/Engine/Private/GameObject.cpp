#include "GameObject.h"
#include "GameInstance.h"

CGameObject::CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance { CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

CGameObject::CGameObject(const CGameObject& Prototype)
	: m_pDevice { Prototype.m_pDevice }
	, m_pContext { Prototype.m_pContext }
	, m_pGameInstance{ CGameInstance::Get_Instance() }
	, m_bCloned{ true }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

CComponent* CGameObject::Get_Component(const _wstring& strComponentTag)
{
	auto	iter = m_Components.find(strComponentTag);
	if (iter == m_Components.end())
		return nullptr;

	return iter->second;
}

HRESULT CGameObject::Initialize_Prototype()
{	

	return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{
	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixIdentity());

	m_pTransformCom = CTransform::Create(m_pDevice, m_pContext);
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	Safe_AddRef(m_pTransformCom);

	m_Components.emplace(g_strTransformTag, m_pTransformCom);

	if (nullptr == pArg)
		return S_OK;

	GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);	
	lstrcpy(m_szName, pDesc->szName);
	m_bActive = pDesc->bActive;
	m_bVisible = pDesc->bVisible;
	m_pParentWorldMatrix = pDesc->pParentWorldMatrix;
	m_iID = pDesc->iID;
	m_pParent = pDesc->pParent;

	if (FAILED(m_pTransformCom->Initialize(pArg)))
		return E_FAIL;


	return S_OK;
}

void CGameObject::Priority_Update(_float fTimeDelta)
{
	for (auto& Pair : m_PartObjects)
	{
		if (nullptr != Pair.second && Pair.second->Get_isActive())
			Pair.second->Priority_Update(fTimeDelta);
	}
}

void CGameObject::Update(_float fTimeDelta)
{
	for (auto& Pair : m_PartObjects)
	{
		if (nullptr != Pair.second && Pair.second->Get_isActive())
			Pair.second->Update(fTimeDelta);
	}
}

void CGameObject::Late_Update(_float fTimeDelta)
{
	for (auto& Pair : m_PartObjects)
	{
		if (nullptr != Pair.second && Pair.second->Get_isActive())
			Pair.second->Late_Update(fTimeDelta);
	}
}

HRESULT CGameObject::Render()
{

	return S_OK;
}

_float CGameObject::Get_Depth()
{
	_vector vWorldPos = {};
	_matrix matView = {};
	_vector vViewSpacePos = {};

	//파츠 들은 컴바인드매트릭스로 계산해야함
	if (m_pParent)
	{
		vWorldPos = XMVectorSet(m_CombinedWorldMatrix._41, m_CombinedWorldMatrix._42, m_CombinedWorldMatrix._43, 1.f);
	}
	else
	{
		vWorldPos = m_pTransformCom->Get_State(STATE::POSITION);
	}

	matView = m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW);
	vViewSpacePos = XMVector3TransformCoord(vWorldPos, matView);
	
	return XMVectorGetZ(vViewSpacePos);

}

CGameObject* CGameObject::Find_PartObject(const _wstring& strPartObjectTag)
{
	auto	iter = m_PartObjects.find(strPartObjectTag);

	if (iter == m_PartObjects.end())
		return nullptr;

	return iter->second;
}

void CGameObject::On_RaycastHit(CGameObject* pOther)
{
}

void CGameObject::On_Collision_Stay(CGameObject* pOther, _fvector vNormal)
{
}

CComponent* CGameObject::Find_Part_Component(const _wstring& strPartObjectTag, const _wstring& strComponentTag)
{
	CGameObject* pPartObject = Find_PartObject(strPartObjectTag);
	if (nullptr == pPartObject)
		return nullptr;

	CComponent* pPartComponent = pPartObject->Get_Component(strComponentTag);
	if (nullptr == pPartComponent)
		return nullptr;

	return pPartComponent;
}

HRESULT CGameObject::Add_PartObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strPartObjectTag, void* pArg)
{
	if (nullptr != Find_PartObject(strPartObjectTag))
		return E_FAIL;

	CGameObject* pPartObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pPartObject)
		return E_FAIL;

	m_PartObjects.emplace(strPartObjectTag, pPartObject);

	return S_OK;
}

HRESULT CGameObject::Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, CComponent** ppOut, void* pArg)
{
	CComponent*	pComponent = static_cast<CComponent*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_COMPONENT, iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pComponent)
		return E_FAIL;

	m_Components.emplace(strComponentTag, pComponent);

	*ppOut = pComponent;

	Safe_AddRef(pComponent);

	return S_OK;
}

HRESULT CGameObject::Remove_Component(const _wstring& strComponentTag)
{
	CComponent* pCom = Get_Component(strComponentTag);

	if (pCom == nullptr)
		return E_FAIL;

	m_Components.erase(strComponentTag);

	Safe_Release(pCom);

	return S_OK;
}

void CGameObject::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pTransformCom);

	for (auto& Pair : m_Components)
		Safe_Release(Pair.second);
	m_Components.clear();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
