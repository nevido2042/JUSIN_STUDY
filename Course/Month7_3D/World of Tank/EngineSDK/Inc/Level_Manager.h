#pragma once

#include "Base.h"

/* ���� ������ �ı��ϰ� ���Ӱ� �Ҵ�� ������ �ּҸ� ��� �ִ´�. */
/* ���� �۾��� ���� ��, ���� ������ �ڿ��� ��� �ı��Ѵ�. */
/* ���� �Ҵ�� ������ �����ϰ� �����ش�. */

NS_BEGIN(Engine)

class CLevel_Manager final : public CBase
{
private:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;

public:
	void Update(_float fTimeDelta);
	HRESULT Render();

public:
	HRESULT		Change_Level(_uint iNewLevelIndex);
	HRESULT		Change_Level(_uint iLevelIndex, class CLevel* pNewLevel);

private:
	_uint					m_iCurrentLevelIndex = {};
	class CLevel*			m_pCurrentLevel = { nullptr };
	class CGameInstance*	m_pGameInstance = { nullptr };

public:
	static CLevel_Manager* Create();
	virtual void Free() override;
};

NS_END