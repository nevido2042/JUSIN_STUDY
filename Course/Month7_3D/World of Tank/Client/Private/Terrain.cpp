#include "Terrain.h"

#include "GameInstance.h"

CTerrain::CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject { pDevice, pContext }
{

}

CTerrain::CTerrain(const CTerrain& Prototype)
	: CGameObject (Prototype)
{

}

HRESULT CTerrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTerrain::Initialize(void* pArg)
{
	GAMEOBJECT_DESC			Desc{};

	Desc.fRotationPerSec = 0.f;
	Desc.fSpeedPerSec = 0.f;
	lstrcpy(Desc.szName, TEXT("Terrain"));	

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CTerrain::Priority_Update(_float fTimeDelta)
{
	//if (m_pGameInstance->Mouse_Pressing(ENUM_CLASS(DIMK::LBUTTON)))
	//{
	//	m_vPickedPos = m_pVIBufferCom->Compute_PickedPosition(m_pTransformCom->Get_WorldMatrix_Inverse());
	//}

	if (m_pGameInstance->Mouse_Pressing(ENUM_CLASS(DIMK::LBUTTON)))
	{
		// 뷰, 프로젝션 행렬
		XMMATRIX matView = m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW);
		XMMATRIX matProj = m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ);

		// 역행렬 (뷰 → 월드)
		XMMATRIX matViewInv = XMMatrixInverse(nullptr, matView);

		// 윈도우 크기
		LONG iWinCX = g_iWinSizeX;
		LONG iWinCY = g_iWinSizeY;

		// 마우스 좌표 (스크린 기준)
		POINT ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse); // DX11에서도 클라이언트 기준으로 보통 변환 필요

		// NDC 좌표로 변환
		float fNDC_X = (2.0f * ptMouse.x / iWinCX - 1.0f);
		float fNDC_Y = (1.0f - 2.0f * ptMouse.y / iWinCY);

		// 뷰 공간에서의 Ray 방향
		XMVECTOR vRayDirView = XMVectorSet(
			fNDC_X / matProj.r[0].m128_f32[0],   // matProj._11
			fNDC_Y / matProj.r[1].m128_f32[1],   // matProj._22
			1.0f,
			0.0f
		);

		// 월드 공간으로 변환
		XMVECTOR vRayOrigin = matViewInv.r[3];  // 카메라 위치 (w=1.0)
		XMVECTOR vRayDir = XMVector3TransformNormal(vRayDirView, matViewInv);
		vRayDir = XMVector3Normalize(vRayDir);

		// 결과 저장 (선택)
		_float3 rayOrigin, rayDir;
		XMStoreFloat3(&rayOrigin, vRayOrigin);
		XMStoreFloat3(&rayDir, vRayDir);


		// 쿼드트리 픽킹
		_float fNearestDist = FLT_MAX;
		_uint iPickedTri = 0;
		if (m_pVIBufferCom->PickQuadTreeNode(rayOrigin, rayDir, fNearestDist, iPickedTri))
		{
			// 충돌 위치 계산
			XMStoreFloat3(&m_vPickedPos, XMLoadFloat3(&rayOrigin) + XMLoadFloat3(&rayDir) * fNearestDist);

			// (선택) 디버그용 출력
			// m_pGameInstance->Draw_Sphere(m_vPickedPos, 0.2f, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
		}
	}
}

void CTerrain::Update(_float fTimeDelta)
{

}

void CTerrain::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CTerrain::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	/* dx9 : 장치에 뷰, 투영행렬을 저장해두면 렌더링시 알아서 정점에 Transform해주었다. */
	/* dx11 : 셰이더에 뷰, 투영행렬을 저장해두고 우리가 직접 변환해주어야한다. */

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	//_float2 TexOffset = { -0.1f, 0.01f };
	//if (FAILED(m_pShaderCom->Bind_TexOffset("g_TexOffset", &TexOffset)))
	//	return E_FAIL;

	_float2 TexScale = { 1.f, 1.f };
	if (FAILED(m_pShaderCom->Bind_TexScale("g_TexScale", &TexScale)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxNorTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Terrain"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Texture_Terrain"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CTerrain* CTerrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTerrain* pInstance = new CTerrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTerrain::Clone(void* pArg)
{
	CTerrain* pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
