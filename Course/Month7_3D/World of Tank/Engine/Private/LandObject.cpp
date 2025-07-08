#include "LandObject.h"

#include "GameInstance.h"


CLandObject::CLandObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice,pContext)
{
}

CLandObject::CLandObject(const CLandObject& Prototype)
	: CGameObject(Prototype)
{
}

HRESULT CLandObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLandObject::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	LANDOBJECT_DESC* pDesc = static_cast<LANDOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTargetBuffer = static_cast<CVIBuffer*>(m_pGameInstance->Get_Component(pDesc->iLevelIndex, pDesc->strLayerTag, pDesc->strComponentTag, pDesc->iIndex));
	Safe_AddRef(m_pTargetBuffer);
	if (nullptr == m_pTargetBuffer)
		return E_FAIL;

	return S_OK;
}

void CLandObject::Priority_Update(_float fTimeDelta)
{
}

void CLandObject::Update(_float fTimeDelta)
{
}

void CLandObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CLandObject::Render()
{
	return S_OK;
}

void CLandObject::SetUp_Height(CTransform* pTransform)
{
	// 현재 위치에서 지형 높이 계산
	_vector vPosition = pTransform->Get_State(STATE::POSITION);
	vPosition = m_pTargetBuffer->Compute_HeightPosition(vPosition);

	// 최종 위치 설정
	vPosition = XMVectorSetY(vPosition, XMVectorGetY(vPosition));
	pTransform->Set_State(STATE::POSITION, vPosition);
}

void CLandObject::Free()
{
	__super::Free();

	Safe_Release(m_pTargetBuffer);
}
