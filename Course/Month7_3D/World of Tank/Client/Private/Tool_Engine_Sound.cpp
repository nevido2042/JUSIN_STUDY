#include "Tool_Engine_Sound.h"

#include "GameInstance.h"
#include "Engine.h"

CTool_Engine_Sound::CTool_Engine_Sound(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTool(pDevice, pContext)
{
}

CTool_Engine_Sound::CTool_Engine_Sound(const CTool_Engine_Sound& Prototype)
	: CTool(Prototype)
{
}

HRESULT CTool_Engine_Sound::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTool_Engine_Sound::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pEngine = static_cast<CEngine*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::PRACTICE), TEXT("Layer_Engine"), 0));
	if (nullptr == m_pEngine)
		return E_FAIL;
	Safe_AddRef(m_pEngine);


	return S_OK;
}

void CTool_Engine_Sound::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CTool_Engine_Sound::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);


	//하이어라키 창
	ImGui::Begin("Tool_Engine_Sound", nullptr);
	ImGui::SliderFloat("RPM", m_pEngine->Get_RPM_Ptr(), 0.0f, m_pEngine->Get_RPM_Max());
	ImGui::SliderFloat("Volume Value", m_pEngine->Get_VolumeValue_Ptr(), 0.0f, 2.f);
	ImGui::SliderFloat("Pitch Value", m_pEngine->Get_PitchValue_Ptr(), 0.0f, 0.1f);
	ImGui::End();


	//ImGui::ShowDemoWindow();

}

void CTool_Engine_Sound::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);


}

HRESULT CTool_Engine_Sound::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CTool_Engine_Sound::Ready_Components()
{
	// 필요한 컴포넌트들 준비
	return S_OK;
}

CTool_Engine_Sound* CTool_Engine_Sound::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTool_Engine_Sound* pInstance = new CTool_Engine_Sound(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEngine_Sound_Tool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTool_Engine_Sound::Clone(void* pArg)
{
	CTool_Engine_Sound* pInstance = new CTool_Engine_Sound(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEngine_Sound_Tool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTool_Engine_Sound::Free()
{
	__super::Free();

	Safe_Release(m_pEngine);
}
