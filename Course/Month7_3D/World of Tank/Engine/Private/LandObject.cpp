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


void CLandObject::SetUp_Height_Normal(CTransform* pTransform, _float fTimeDelta, _float fOffset)
{
	// ���� ��ġ���� ���� ���� ���
	_vector vPosition = pTransform->Get_State(STATE::POSITION);
	vPosition = m_pTargetBuffer->Compute_HeightPosition(vPosition);

	// ������ ���� ��ǥ��
#pragma message("������ �ܺο��� �޾ƿͼ� �����ϸ� ���� ��")
	const _float fSampleOffset = 1.0f;

	_vector vOffsets[4] = {
	XMVectorSet(+fSampleOffset, 0.f, +fSampleOffset * 2.f, 0.f),
	XMVectorSet(-fSampleOffset, 0.f, +fSampleOffset * 2.f, 0.f),
	XMVectorSet(+fSampleOffset, 0.f, -fSampleOffset * 2.f, 0.f),
	XMVectorSet(-fSampleOffset, 0.f, -fSampleOffset * 2.f, 0.f)
	};

	// 4�� ��ġ �븻 ���
	_vector vNormalSum = XMVectorZero();
	for (int i = 0; i < 4; ++i)
	{
		_vector vSamplePos = vPosition + vOffsets[i];
		vSamplePos = m_pTargetBuffer->Compute_HeightPosition(vSamplePos);
		vNormalSum += m_pTargetBuffer->Compute_NormalPosition(vSamplePos);
	}
	_vector vTargetUp = XMVector3Normalize(vNormalSum / 4.f);

	// ���� Up ���� ��������
	_vector vCurrentUp = pTransform->Get_State(STATE::UP);

	// ���� �������� �ε巴�� ��ȭ
	const _float fLerpSpeed = 5.f; // Ŭ���� ������ ������
	vTargetUp = XMVector3Normalize(XMVectorLerp(vCurrentUp, vTargetUp, fLerpSpeed * fTimeDelta));

	// Look ���ʹ� ���� ����
	_vector vLook = XMVector3Normalize(pTransform->Get_State(STATE::LOOK));

	// Up�� Look���κ��� Right ����
	_vector vRight = XMVector3Normalize(XMVector3Cross(vTargetUp, vLook));

	// Right�� Up���� Look ����
	vLook = XMVector3Normalize(XMVector3Cross(vRight, vTargetUp));

	// ���� ��ġ ����
	vPosition = XMVectorSetY(vPosition, XMVectorGetY(vPosition) + fOffset);
	pTransform->Set_State(STATE::POSITION, vPosition);

	// ���� ���� ����
	pTransform->Set_State(STATE::RIGHT, vRight);
	pTransform->Set_State(STATE::UP, vTargetUp);
	pTransform->Set_State(STATE::LOOK, vLook);


}

void CLandObject::Free()
{
	__super::Free();

	Safe_Release(m_pTargetBuffer);
}
