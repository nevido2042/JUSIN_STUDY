#include "GameTimer.h"

#include "GameInstance.h"

#include "GameManager.h"

#include "Camera_Missile.h"

CGameTimer::CGameTimer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CGameTimer::CGameTimer(const CGameTimer& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CGameTimer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGameTimer::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	CGameManager* pGameManager = GET_GAMEMANAGER;
	if (pGameManager)
	{
		m_GameEndTime = pGameManager->Get_GameEndTime();

		m_GameStartTime = m_GameEndTime - chrono::seconds(20); //대기 10초, 게임시간 20초 (총 30초)

		pGameManager->Set_GameStartTime(m_GameStartTime);
	}

	return S_OK;
}

void CGameTimer::Priority_Update(_float fTimeDelta)
{

}

void CGameTimer::Update(_float fTimeDelta)
{
	// 게임 시작 시간이 아직 안 지났다면
	if (m_GameStartTime > chrono::steady_clock::now())
	{
		//게임 시작 시간까지 남은 시간을 계산
		m_Remaining = chrono::duration_cast<chrono::seconds>(m_GameStartTime - chrono::steady_clock::now());
	}
	else
	{
		//게임 종료 까지 남은 시간을 계산
		m_Remaining = chrono::duration_cast<chrono::seconds>(m_GameEndTime - chrono::steady_clock::now());

		if (max<_int>(0, static_cast<_int>(m_Remaining.count())) == 0)
		{
			if (m_bIsSpawnMissile == false)
			{
				m_bIsSpawnMissile = true;

				//GAMEOBJECT_DESC Desc = {};
				//Desc.vInitPosition = _float3(TERRAIN_SIZE * TERRAIN_OFFSET_WIDTH * 0.5f, 300.f, TERRAIN_SIZE * TERRAIN_OFFSET_WIDTH * 0.5f);
				//m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Missile"), m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Missile"), &Desc);

				CGameObject::GAMEOBJECT_DESC MissileDesc = {};
				MissileDesc.vInitPosition = _float3(0.f, 110.f, 0.f);
				m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Missile"), m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Missile"), &MissileDesc);

				CCamera_Missile::CAMERA_MISSILE_DESC Desc{};
				Desc.pTarget = m_pGameInstance->Get_Last_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Missile"));
				Desc.bActive = true;
				lstrcpy(Desc.szName, TEXT("Camera_Missile"));

				if (FAILED(m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera_Missile"),
					ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Camera_Missile"), &Desc)))
					return;
			}
		}
	}

}

void CGameTimer::Late_Update(_float fTimeDelta)
{
	if (m_bActive)
		m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);
}

HRESULT CGameTimer::Render()
{

	// 음수 방지 (시간이 지났으면 0초로 표시)
	_int iTotalSeconds = max<_int>(0, static_cast<_int>(m_Remaining.count()));

	_uint iMin = iTotalSeconds / 60;
	_uint iSec = iTotalSeconds % 60;

	wstringstream ss;
	ss << setfill(L'0') << setw(2) << iMin << L":" << setfill(L'0') << setw(2) << iSec;

	wstring strText = ss.str();

	// 숫자를 문자열로 변환
	//wstring strText = to_wstring(iMin) + L':' + to_wstring(iSec);

	// 폰트 출력
	m_pGameInstance->Draw_Font(TEXT("Font_WarheliosKO"), strText.c_str(), _float2(g_iWinSizeX * 0.96f, 0), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f), 0.3f * UI_RATIO);

	return S_OK;
}


CGameTimer* CGameTimer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGameTimer* pInstance = new CGameTimer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGameTimer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGameTimer::Clone(void* pArg)
{
	CGameTimer* pInstance = new CGameTimer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGameTimer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGameTimer::Free()
{
	__super::Free();
}
