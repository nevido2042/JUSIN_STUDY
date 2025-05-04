#include "Level_Loading.h"


#include "Level_GamePlay.h"
#include "Level_Logo.h"
#include "Loader.h"

#include "GameInstance.h"

CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
		: CLevel { pDevice, pContext }
{

}

HRESULT CLevel_Loading::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	/* �ε����� ��ü�� �ʿ��� ��ü�� �����Ѵ�. */
	/* ���, �ε���, ��ư, font */

	/* �ε��� ����(���������� �ʿ��� �ڿ�(Resource)(�ؽ���, ��, ���� ���� )�� �����ϴ�)�� 
	������ �δ���ü�� �����Ѵ�. */
	m_pLoader = CLoader::Create(m_pDevice, m_pContext, m_eNextLevelID);
	if (nullptr == m_pLoader)
		return E_FAIL;
	
	return S_OK;
}

void CLevel_Loading::Update(_float fTimeDelta)
{

	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		if (true == m_pLoader->isFinished())
		{
			CLevel* pLevel = { nullptr };

			switch (m_eNextLevelID)
			{
			case LEVEL::LEVEL_LOGO:
				pLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
				break;
			case LEVEL::LEVEL_GAMEPLAY:
				pLevel = CLevel_GamePlay::Create(m_pDevice, m_pContext);
				break;
			}

			if (nullptr == pLevel)
				return;

			if (FAILED(m_pGameInstance->Change_Level(static_cast<_uint>(m_eNextLevelID), pLevel)))
				return;
							
		}
	}	
}

HRESULT CLevel_Loading::Render()
{
	m_pLoader->Output_LoadingText();

	return S_OK;
}

CLevel_Loading* CLevel_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID)
{
	CLevel_Loading* pInstance = new CLevel_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed to Created : CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}
