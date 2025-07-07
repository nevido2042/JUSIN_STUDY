#include "Score.h"

#include "GameInstance.h"
#include "Score_Tank.h"
#include "Tank.h"
#include "Layer.h"

CScore::CScore(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{

}

CScore::CScore(const CScore& Prototype)
	: CUIObject(Prototype)
{

}

HRESULT CScore::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CScore::Initialize(void* pArg)
{
	UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	return S_OK;
}

void CScore::Priority_Update(_float fTimeDelta)
{
	if (g_bWindowResizeRequired)
		Resize(g_iWinSizeX, g_iWinSizeY);

	CGameObject::Priority_Update(fTimeDelta);
}

void CScore::Update(_float fTimeDelta)
{
	//게임이 끝났으면
	if (m_bIsGameEnd)
	{
		if (m_bIsSpawnMissile == false)
		{
			m_bIsSpawnMissile = true;

			GAMEOBJECT_DESC Desc = {};
			Desc.vInitPosition = _float3(TERRAIN_SIZE * TERRAIN_OFFSET_WIDTH * 0.5f, 100.f, TERRAIN_SIZE * TERRAIN_OFFSET_WIDTH * 0.5f);
			m_pGameInstance->Add_GameObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Missile"), m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Missile"), &Desc);
		}

		m_fAccTime += fTimeDelta;

		//5초간 대기하다가 격납고로 이동
		if (m_fAccTime > 5.f)
		{
			//m_pGameInstance->Change_Level(ENUM_CLASS(LEVEL::HANGER));
		}
	}

	CGameObject::Update(fTimeDelta);
}

void CScore::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_UI, this);

	CGameObject::Late_Update(fTimeDelta);

}

HRESULT CScore::Render()
{
	_tchar szScore[MAX_PATH];
	_stprintf_s(szScore, _T("%d : %d"), m_iGreenScore, m_iRedScore);

	// 폰트 출력
	m_pGameInstance->Draw_Font(TEXT("Font_WarheliosKO"), szScore, _float2(g_iWinSizeX * 0.484f, g_iWinSizeY * 0.01f), XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f), 0.4f * UI_RATIO);

	if (m_bIsGameEnd)
	{
		Draw_GameResult();
	}

	return S_OK;
}

void CScore::Set_Destroy_Green()
{
	if (m_bIsGameEnd)
		return;

	_tchar szPartName[MAX_PATH];
	_stprintf_s(szPartName, _T("Part_Green%d"), m_iNumGreen - 1);

	CGameObject* pGameObject = Find_PartObject(szPartName);
	if (!pGameObject)
		return;

	--m_iNumGreen;
	++m_iRedScore;

	static_cast<CScore_Tank*>(pGameObject)->Set_ColorDark();

	Check_Team_All_Dead();
}

void CScore::Set_Destroy_Red()
{
	if (m_bIsGameEnd)
		return;

	_tchar szPartName[MAX_PATH];
	_stprintf_s(szPartName, _T("Part_Red%d"), m_iNumRed - 1);


	CGameObject* pGameObject = Find_PartObject(szPartName);
	if (!pGameObject)
		return;

	--m_iNumRed;
	++m_iGreenScore;

	static_cast<CScore_Tank*>(pGameObject)->Set_ColorDark();

	Check_Team_All_Dead();
}

void CScore::Check_Team_All_Dead()
{
	//어느 한쪽도 전멸하지 않았다.
	if (m_iNumRed != 0 && m_iNumGreen != 0)
		return;

	m_bIsGameEnd = true;
}

void CScore::Draw_GameResult()
{
	_float2 vPos = _float2(g_iWinSizeX * 0.4f, g_iWinSizeY * 0.1f);
	_float fSize = 2.f * UI_RATIO;

	//승리 패배 출력해라
	if (m_iNumGreen == 0)
	{
		//패배
		// 폰트 출력
		m_pGameInstance->Draw_Font(TEXT("Font_WarheliosKO"), TEXT("패배"), vPos, XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f), fSize);
	}
	else
	{
		//승리
		// 폰트 출력
		m_pGameInstance->Draw_Font(TEXT("Font_WarheliosKO"), TEXT("승리"), vPos, XMVectorSet(1.f, 1.f, 1.f, 1.f), 0.f, _float2(0.f, 0.f), fSize);
	}
}

HRESULT CScore::Ready_Components()
{


	return S_OK;
}

HRESULT CScore::Ready_PartObjects()
{
	CScore_Tank::SCORE_TANK_DESC Score_Tank_Desc{};
	Score_Tank_Desc.fDepth = DEPTH_BACKGROUND - 0.1f;
	Score_Tank_Desc.bIsChild = true;
	lstrcpy(Score_Tank_Desc.szName, TEXT("Score_Tank"));
	Score_Tank_Desc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();

	m_iNumGreen = 0;
	m_iNumRed = 0;
	//플레이어의 탱크의 팀 확인하고 그린쪽 하나 추가
	CGameObject* pGameOject = m_pGameInstance->Get_Last_GameObject(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_PlayerTank"));

	if (pGameOject == nullptr)
		return E_FAIL;

	TEAM ePlayerTeam = static_cast<CTank*>(pGameOject)->Get_Team();

	lstrcpy(Score_Tank_Desc.szTextureName, TEXT("Prototype_Component_Texture_Score_Tank_Green"));
	Score_Tank_Desc.fX = -1.f + m_iNumGreen * -0.6f;
	_tchar szPartName[MAX_PATH];
	_stprintf_s(szPartName, _T("Part_Green%d"), m_iNumGreen);
	if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Score_Tank"), szPartName, &Score_Tank_Desc)))
		return E_FAIL;
	++m_iNumGreen;
	
	//탱크레이어에서 플레이어와 같은 탱크가 있으면 그린쪽 으로 추가
	//반대는 빨간색으로 추가

	CLayer* pLayer = m_pGameInstance->Find_Layer(m_pGameInstance->Get_NewLevel_Index(), TEXT("Layer_Tank"));
	if (nullptr == pLayer)
		return S_OK;

	for (CGameObject* pGameObject : pLayer->Get_GameObjects())
	{
		TEAM eTeam = static_cast<CTank*>(pGameObject)->Get_Team();

		if (eTeam == ePlayerTeam)
		{
			lstrcpy(Score_Tank_Desc.szTextureName, TEXT("Prototype_Component_Texture_Score_Tank_Green"));
			Score_Tank_Desc.fX = -1.f + m_iNumGreen * -0.6f;
			_stprintf_s(szPartName, _T("Part_Green%d"), m_iNumGreen);
			if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Score_Tank"), szPartName, &Score_Tank_Desc)))
				return E_FAIL;
			++m_iNumGreen;
		}
		else
		{
			lstrcpy(Score_Tank_Desc.szTextureName, TEXT("Prototype_Component_Texture_Score_Tank_Red"));
			Score_Tank_Desc.fX = 1.f + m_iNumRed * 0.6f;
			_stprintf_s(szPartName, _T("Part_Red%d"), m_iNumRed);
			if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Score_Tank"), szPartName, &Score_Tank_Desc)))
				return E_FAIL;
			++m_iNumRed;
		}
	}


	//lstrcpy(Score_Tank_Desc.szTextureName, TEXT("Prototype_Component_Texture_Score_Tank_Green"));
	//Score_Tank_Desc.fX = -1.f;
	//if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Score_Tank"), TEXT("Part_Green0"), &Score_Tank_Desc)))
	//	return E_FAIL;

	//lstrcpy(Score_Tank_Desc.szTextureName, TEXT("Prototype_Component_Texture_Score_Tank_Red"));
	//Score_Tank_Desc.fX = 1.f;
	//if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Score_Tank"), TEXT("Part_Red0"), &Score_Tank_Desc)))
	//	return E_FAIL;

	//lstrcpy(Score_Tank_Desc.szTextureName, TEXT("Prototype_Component_Texture_Score_Tank_Red"));
	//Score_Tank_Desc.fX = 1.6f;
	//if (FAILED(__super::Add_PartObject(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Score_Tank"), TEXT("Part_Red1"), &Score_Tank_Desc)))
	//	return E_FAIL;

	return S_OK;
}

CScore* CScore::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CScore* pInstance = new CScore(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CScore");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CScore::Clone(void* pArg)
{
	CScore* pInstance = new CScore(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CScore");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CScore::Free()
{
	__super::Free();

	for (auto& Pair : m_PartObjects)
		Safe_Release(Pair.second);
	m_PartObjects.clear();
}
