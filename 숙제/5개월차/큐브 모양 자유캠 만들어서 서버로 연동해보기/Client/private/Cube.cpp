#include "Cube.h"

#include "Transform.h"
#include "GameInstance.h"

//#include "PacketHandler.h"

CCube::CCube(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CCube::CCube(const CCube& Prototype)
	: CGameObject{ Prototype },
	m_pNetwork{ CNetwork::Get_Instance() }
{
	Safe_AddRef(m_pNetwork);
}

HRESULT CCube::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCube::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	//m_pNetwork = CNetwork::Create();
	CUBE_DESC tDesc = *static_cast<CUBE_DESC*>(pArg);
	m_iMyID = tDesc.iID;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, tDesc.Postion);

	return S_OK;
}

void CCube::Priority_Update(_float fTimeDelta)
{
	int a = 10;
}

void CCube::Update(_float fTimeDelta)
{
	if (m_IsMove)
	{
		m_pTransformCom->Go_Direction(fTimeDelta, m_vMoveDir);
	}

	//네트워크ID와 큐브의 아이디가 다르면 입력 받지 못하게
	if (!m_pNetwork->Compare_ID(m_iMyID))
	{
		return;
	}
	
	if (g_hWnd != GetForegroundWindow()) return;

	if(CGameInstance::Get_Instance()->Key_Pressing(VK_UP))
	{
		m_pTransformCom->Go_Straight(0.016f);
	}
	if (CGameInstance::Get_Instance()->Key_Pressing(VK_DOWN))
	{
		m_pTransformCom->Go_Backward(0.016f);
	}
	if (CGameInstance::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		m_pTransformCom->Go_Left(0.016f);
	}
	if (CGameInstance::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		m_pTransformCom->Go_Right(0.016f);
	}

	//키를 땟을 때
	if (CGameInstance::Get_Instance()->Key_Up(VK_UP) ||
		CGameInstance::Get_Instance()->Key_Up(VK_DOWN) ||
		CGameInstance::Get_Instance()->Key_Up(VK_LEFT) ||
		CGameInstance::Get_Instance()->Key_Up(VK_RIGHT))
	{
		//아무것도 안눌렀을 때 스탑을 보내는게 맞음.
		if (!CGameInstance::Get_Instance()->Key_Pressing(VK_UP) &&
			!CGameInstance::Get_Instance()->Key_Pressing(VK_DOWN) &&
			!CGameInstance::Get_Instance()->Key_Pressing(VK_LEFT) &&
			!CGameInstance::Get_Instance()->Key_Pressing(VK_RIGHT))
		{
			m_pNetwork->mp_CS_Move_Stop(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
		}
	}
}

void CCube::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this)))
		return;

	//네트워크ID와 큐브의 아이디가 다르면 입력 받지 못하게
	if (!m_pNetwork->Compare_ID(m_iMyID))
	{
		return;
	}

	if (g_hWnd != GetForegroundWindow()) return;

	_float3 MoveDirection = m_pTransformCom->Get_State(CTransform::STATE_POSITION) - m_OldPosition;
	D3DXVec3Normalize(&MoveDirection, &MoveDirection);

	//이걸 레이트업데이트 호출 해서 (oldPos - currentPos)방향을 전달하는게 맞을지도
	if (CGameInstance::Get_Instance()->Key_Down(VK_UP)||
		CGameInstance::Get_Instance()->Key_Down(VK_DOWN)||
		CGameInstance::Get_Instance()->Key_Down(VK_LEFT)||
		CGameInstance::Get_Instance()->Key_Down(VK_RIGHT))
	{
		//VK_UP 시작
		//현재위치 전달, 움직이는 방향 전달
		m_pNetwork->mp_CS_Move_Start(m_pTransformCom->Get_State(CTransform::STATE_POSITION), MoveDirection);
		//m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	}

	m_OldPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

}

HRESULT CCube::Render()
{
	/* 장치에 텍스쳐를 하나 저장해 놓는다. */
	/* 추후 렌더링하는데 이용하는 정점이 텍스쿠드를 들고 있다면 */
	/* 해당 정점으로 인해 생성된 픽셀에게 샘플링되기위한 텍스쳐다. */
	if (FAILED(m_pTransformCom->Bind_Resource()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Resource(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	/* 정점을 그린다. */
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	return S_OK;
}

int CCube::Get_ID()
{
	return m_iMyID;
}

CTransform* CCube::Get_TransformCom()
{
	return m_pTransformCom;
}

void CCube::Set_Move(bool _bool)
{
	m_IsMove = _bool;
}

void CCube::Set_MoveDirection(_float3& vMoveDir)
{
	m_vMoveDir = vMoveDir;
}

HRESULT CCube::Ready_Components()
{
	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Cube"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	///* For.Com_Transform */
	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
	//	TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
	//	return E_FAIL;

	CTransform::TRANSFORM_DESC		TransformDesc{};

	TransformDesc.fSpeedPerSec = 30.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(180.f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;



	return S_OK;
}

CCube* CCube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCube* pInstance = new CCube(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCube::Clone(void* pArg)
{
	CCube* pInstance = new CCube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCube");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCube::Free()
{
	__super::Free();

	//m_pNetwork->Release();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);

	Safe_Release(m_pNetwork);


}
