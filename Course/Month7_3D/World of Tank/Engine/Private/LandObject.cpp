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
	// 현재 위치에서 지형 높이 계산
	_vector vPosition = pTransform->Get_State(STATE::POSITION);
	vPosition = m_pTargetBuffer->Compute_HeightPosition(vPosition);

	// 오프셋 적용 좌표들
#pragma message("오프셋 외부에서 받아와서 적용하면 좋을 듯")
	const _float fSampleOffset = 1.0f;

	_vector vOffsets[4] = {
	XMVectorSet(+fSampleOffset, 0.f, +fSampleOffset * 2.f, 0.f),
	XMVectorSet(-fSampleOffset, 0.f, +fSampleOffset * 2.f, 0.f),
	XMVectorSet(+fSampleOffset, 0.f, -fSampleOffset * 2.f, 0.f),
	XMVectorSet(-fSampleOffset, 0.f, -fSampleOffset * 2.f, 0.f)
	};

	// 4개 위치 노말 평균
	_vector vNormalSum = XMVectorZero();
	for (int i = 0; i < 4; ++i)
	{
		_vector vSamplePos = vPosition + vOffsets[i];
		vSamplePos = m_pTargetBuffer->Compute_HeightPosition(vSamplePos);
		vNormalSum += m_pTargetBuffer->Compute_NormalPosition(vSamplePos);
	}
	_vector vTargetUp = XMVector3Normalize(vNormalSum / 4.f);

	// 이전 Up 벡터 가져오기
	_vector vCurrentUp = pTransform->Get_State(STATE::UP);

	// 선형 보간으로 부드럽게 변화
	const _float fLerpSpeed = 5.f; // 클수록 빠르게 보간됨
	vTargetUp = XMVector3Normalize(XMVectorLerp(vCurrentUp, vTargetUp, fLerpSpeed * fTimeDelta));

	// Look 벡터는 이전 유지
	_vector vLook = XMVector3Normalize(pTransform->Get_State(STATE::LOOK));

	// Up과 Look으로부터 Right 재계산
	_vector vRight = XMVector3Normalize(XMVector3Cross(vTargetUp, vLook));

	// Right와 Up으로 Look 재계산
	vLook = XMVector3Normalize(XMVector3Cross(vRight, vTargetUp));

	// 최종 위치 설정
	vPosition = XMVectorSetY(vPosition, XMVectorGetY(vPosition) + fOffset);
	pTransform->Set_State(STATE::POSITION, vPosition);

	// 방향 벡터 적용
	pTransform->Set_State(STATE::RIGHT, vRight);
	pTransform->Set_State(STATE::UP, vTargetUp);
	pTransform->Set_State(STATE::LOOK, vLook);


}

void CLandObject::Free()
{
	__super::Free();

	Safe_Release(m_pTargetBuffer);
}
