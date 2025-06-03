#include "Icon_Consumables.h"

#include "GameInstance.h"
#include "Tank.h"
#include "GameManager.h"

CIcon_Consumables::CIcon_Consumables(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CIcon_Consumables::CIcon_Consumables(const CIcon_Consumables& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CIcon_Consumables::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CIcon_Consumables::Initialize(void* pArg)
{
	ICON_CONSUMABLES_DESC* pDesc = static_cast<ICON_CONSUMABLES_DESC*>(pArg);
	m_strTextureName = pDesc->strTextureName;
	m_iKeyNumber = pDesc->iKeyNumber;
	m_eConsumables = pDesc->eConsumables;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

void CIcon_Consumables::Priority_Update(_float fTimeDelta)
{
	if (g_bWindowResizeRequired)
		Resize(g_iWinSizeX, g_iWinSizeY);
}

void CIcon_Consumables::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down(m_iKeyNumber + 1))
	{
		if(chrono::steady_clock::now() < m_CanUseTime)
			return; // 아직 사용 불가 시간이라면 리턴
		m_CanUseTime = chrono::steady_clock::now() + chrono::seconds(5);
		CGameManager* pGameManager = GET_GAMEMANAGER;
		//해당 아이템 효과 발동
		switch (m_eConsumables)
		{
		case Client::CONSUMABLES::HAND_EXTINGUISHER:
			pGameManager->PlaySound_Extinguisher();
			break;
		case Client::CONSUMABLES::SMALL_REPAIR_KIT:
			static_cast<CTank*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_PlayerTank")))->Repair_All();
			pGameManager->PlaySound_Repair();
			break;
		case Client::CONSUMABLES::SMALL_MED_KIT:
			pGameManager->PlaySound_Medkit();
			break;
		case Client::CONSUMABLES::END:
			break;
		default:
			break;
		}

		

	}
}

void CIcon_Consumables::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CIcon_Consumables::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	_float fAlpha = { 0.3f };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fAlpha", &fAlpha, sizeof(_float))))
		return E_FAIL;



	if (FAILED(m_pTextureCom_Background->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_pGameInstance->Draw_Font(TEXT("Font_WarheliosKO"), to_wstring(m_iKeyNumber).c_str(), _float2(m_fX - m_fSizeX * 0.2f, m_fY - m_fSizeY * 1.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), 0.f, _float2(0.f, 0.f), 0.3f * UI_RATIO);

	// 시간 차이 계산
	chrono::seconds Remaining = chrono::duration_cast<chrono::seconds>(m_CanUseTime - chrono::steady_clock::now());

	// 음수 방지 (시간이 지났으면 0초로 표시)
	_int iDisplaySeconds = max<_int>(0, static_cast<_int>(Remaining.count()));
	
	if (iDisplaySeconds > 0)
	{
		// 숫자를 문자열로 변환
		wstring strText = to_wstring(iDisplaySeconds);
		m_pGameInstance->Draw_Font(TEXT("Font_WarheliosKO"), strText.c_str(), _float2(m_fX - m_fSizeX * 0.2f, m_fY + m_fSizeY * 0.2f), XMVectorSet(1.f, 0.f, 0.f, 1.f), 0.f, _float2(0.f, 0.f), 0.3f * UI_RATIO);

	}


	return S_OK;
}

HRESULT CIcon_Consumables::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), m_strTextureName,
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_Texture_Background */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Back_Ghost"),
		TEXT("Com_Texture_Background"), reinterpret_cast<CComponent**>(&m_pTextureCom_Background))))
		return E_FAIL;

	return S_OK;
}

CIcon_Consumables* CIcon_Consumables::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CIcon_Consumables* pInstance = new CIcon_Consumables(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CIcon_Consumables");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CIcon_Consumables::Clone(void* pArg)
{
	CIcon_Consumables* pInstance = new CIcon_Consumables(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CIcon_Consumables");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CIcon_Consumables::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTextureCom_Background);
}
