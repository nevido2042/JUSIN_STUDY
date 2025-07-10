#include "MapVegetation.h"
#include "GameInstance.h"

CMapVegetation::CMapVegetation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CMapVegetation::CMapVegetation(const CMapVegetation& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CMapVegetation::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMapVegetation::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	Ready_Components();

	Load_Vegetations();

	return S_OK;
}

void CMapVegetation::Priority_Update(_float fTimeDelta)
{
}

void CMapVegetation::Update(_float fTimeDelta)
{
}

void CMapVegetation::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONLIGHT, this);

}

HRESULT CMapVegetation::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(1)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CMapVegetation::Load_Vegetations()
{
	if (m_pGameInstance->Get_NewLevel_Index() != ENUM_CLASS(LEVEL::MAPTOOL))
	{
		vector<_float3> VegeTranslates;

		filesystem::path Path = "../Bin/Map/VegeTranslates.bin";

		FILE* fp = nullptr;
		fopen_s(&fp, Path.string().c_str(), "rb");
		if (!fp)
		{
			cout << "Fail!!!!!! Load_Vegetation" << endl;
			return;
		}

		size_t Count = { 0 };
		fread(&Count, sizeof(size_t), 1, fp);

		VegeTranslates.reserve(Count);

		while (true)
		{
			_float3 vPos = {};
			size_t readCount = fread(&vPos, sizeof(_float3), 1, fp);

			// 더 이상 읽을게 없으면 종료
			if (readCount != 1)
				break;

			VegeTranslates.push_back(vPos);
		}

		m_pVIBufferCom->Change_NumInstance(static_cast<_uint>(VegeTranslates.size()));
		m_pVIBufferCom->Change_Translation(VegeTranslates);
		m_pVIBufferCom->Change_Size_XY(_float2(209.f * 0.01f, 345.f * 0.01f));

		fclose(fp);

		cout << "(Load)Vege Count:" << VegeTranslates.size() << endl;
	}
}

HRESULT CMapVegetation::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_Boundary"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;
	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Boundary"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Grass"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMapVegetation::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

CMapVegetation* CMapVegetation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMapVegetation* pInstance = new CMapVegetation(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMapVegetation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMapVegetation::Clone(void* pArg)
{
	CMapVegetation* pInstance = new CMapVegetation(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMapVegetation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMapVegetation::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
}
