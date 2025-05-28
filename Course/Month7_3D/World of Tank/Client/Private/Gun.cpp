#include "Gun.h"

#include "GameInstance.h"
#include "Terrain.h"

CGun::CGun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice, pContext)
{
}

CGun::CGun(const CGun& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CGun::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CGun::Priority_Update(_float fTimeDelta)
{
	Input(fTimeDelta);

	if (m_pGameInstance->Get_ID() != m_iID && m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::GAMEPLAY))
	{
		// ȸ�� �� (x��)
		_vector vAxis = XMVectorSet(1.f, 0.f, 0.f, 0.f);

		// ���� Look ���� ��������
		_vector vLook = m_pTransformCom->Get_State(STATE::LOOK);
		vLook = XMVector3Normalize(vLook);

		// Pitch = Look ���Ϳ� ���� ���� (XZ ���)�� ����
		// ���⼱ Y���� �������� �Ǵ� ����
		_float fDotY = XMVectorGetY(vLook); // = sin(pitch)

		// ���� ȸ�� �� ����
		if (m_bUp && fDotY <= m_fMaxPitch)
			m_pTransformCom->Turn(vAxis, -fTimeDelta);
		// ���� ȸ�� �� ����
		else if (m_bDown && fDotY >= m_fMinPitch)
			m_pTransformCom->Turn(vAxis, fTimeDelta);
	}
	
}

void CGun::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_ID() == m_iID && GetForegroundWindow() == g_hWnd && m_pGameInstance->Get_NewLevel_Index() != ENUM_CLASS(LEVEL::PRACTICE))
	{
		m_fTimeAcc += fTimeDelta;
		if (m_fTimeAcc > m_fSyncInterval)
		{
			m_fTimeAcc = 0.f;

			MATRIX_DESC Desc{};
			Desc.iID = m_iID;
			XMStoreFloat4x4(&Desc.Matrix, m_pTransformCom->Get_WorldMatrix());

			m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_MATRIX_GUN), &Desc);
		}
	}

	//�θ��� ���� ����� �����ͼ� �ڽ��� ���� ��İ� �����ش�.
	XMStoreFloat4x4(&m_CombinedWorldMatrix, XMMatrixMultiply(m_pTransformCom->Get_WorldMatrix(), XMLoadFloat4x4(m_pParentWorldMatrix)));
}

HRESULT CGun::Fire()
{
	_vector vPos = XMVectorSet(m_CombinedWorldMatrix._41, m_CombinedWorldMatrix._42, m_CombinedWorldMatrix._43, 1.0f);
	m_pSoundCom->Update3DPosition(vPos);

	m_pSoundCom->Play("wpn_1");

	return S_OK;
}

void CGun::Input(_float fTimeDelta)
{
	if (GetForegroundWindow() != g_hWnd)
		return;

	if (m_pGameInstance->Mouse_Pressing(ENUM_CLASS(DIMK::RBUTTON)))
		return;

	if (m_pGameInstance->Get_ID() == m_iID)
	{
		// Ÿ�� ��ġ
		CTerrain* pTerrain = static_cast<CTerrain*>(m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Terrain")));
		if (pTerrain == nullptr)
			return;

		// �� ��ġ
		_float3 vMyPos = {
			m_CombinedWorldMatrix.m[3][0],
			m_CombinedWorldMatrix.m[3][1],
			m_CombinedWorldMatrix.m[3][2]
		};

		// Pick�� ��ġ
		_float3 vPicked = pTerrain->Get_PickedPos();

		// ���� ���� (���� ����)
		_vector vToPicked = XMLoadFloat3(&vPicked) - XMLoadFloat3(&vMyPos);

		// �� Up, Look, Right ���� (����ȭ�� �ִٰ� ����)
		_vector vUp = XMVector3Normalize(XMLoadFloat3((_float3*)&m_CombinedWorldMatrix.m[1]));
		_vector vLook = XMVector3Normalize(XMLoadFloat3((_float3*)&m_CombinedWorldMatrix.m[2]));
		_vector vRight = XMVector3Normalize(XMLoadFloat3((_float3*)&m_CombinedWorldMatrix.m[0]));

		// Look-Up ��鿡 ���� = Right �� ����
		_vector vProjected = vToPicked - XMVector3Dot(vToPicked, vRight) * vRight;  // vToPicked�� Right ���� ����

		vProjected = XMVector3Normalize(vProjected);  // ���⸸ �ʿ��ϴٸ� ����ȭ

		// Up ���Ϳ� ���� ��/�Ʒ� �Ǵ�
		_float fUpDot = XMVectorGetX(XMVector3Dot(vProjected, -vUp));

		// ȸ�� �� (x��)
		_vector vAxis = XMVectorSet(1.f, 0.f, 0.f, 0.f);

		// ���� Look ���� ��������
		_vector vLocalLook = m_pTransformCom->Get_State(STATE::LOOK);
		vLocalLook = XMVector3Normalize(vLocalLook);

		// Pitch = Look ���Ϳ� ���� ���� (XZ ���)�� ����
		// ���⼱ Y���� �������� �Ǵ� ����
		_float fDotY = XMVectorGetY(vLocalLook); // = sin(pitch)

		if (fUpDot > 0.1f) //�Ϸ�
		{
			if (m_bDown == false)
			{
				m_bDown = true;
				m_bUp = false;

				if (m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::GAMEPLAY))
				{
					BOOL_DESC Desc{};
					Desc.iID = m_iID;
					Desc.bBool = true;
					m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_DOWN), &Desc);
				}
			}
			if(fDotY >= m_fMinPitch)
				m_pTransformCom->Turn(vAxis, fTimeDelta);
		}
		else if (fUpDot < -0.1f) //������
		{
			if (m_bUp == false)
			{
				m_bUp = true;
				m_bDown = false;

				if (m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::GAMEPLAY))
				{
					BOOL_DESC Desc{};
					Desc.iID = m_iID;
					Desc.bBool = true;
					m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_UP), &Desc);
				}

			}
			if (fDotY <= m_fMaxPitch)
				m_pTransformCom->Turn(vAxis, -fTimeDelta);
		}
		else
		{
			if (m_bUp || m_bDown)
			{
				m_bUp = m_bDown = false;

				if (m_pGameInstance->Get_NewLevel_Index() == ENUM_CLASS(LEVEL::GAMEPLAY))
				{
					BOOL_DESC Desc{};
					Desc.iID = m_iID;
					Desc.bBool = false;
					m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_UP), &Desc);
					Desc.bBool = false;
					m_pGameInstance->Send_Packet(ENUM_CLASS(PacketType::CS_DOWN), &Desc);
				}

			}
		}
	}
}

void CGun::Free()
{
	__super::Free();

}
