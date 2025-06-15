#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CCollider_Manager final : public CBase
{
private:
	CCollider_Manager();
	virtual ~CCollider_Manager() = default;
public:
	HRESULT Initialize(_uint iNumGroups);

public:
	HRESULT Add_CollisionGroup(_uint iGroupIndex, class CGameObject* pGameObject, wstring strComponentTag);
	HRESULT Out_CollisionGroup(_uint iGroupIndex, class CGameObject* pGameObject);
	HRESULT Clear_CollisionGroups();
public:
	void Check_Collision(_uint iGroupIndex, class CGameObject* pGameObject, wstring strComponentTag, wstring strOtherComponentTag);
	class CGameObject* Check_RaycastHit(_uint iGroupIndex, wstring strComponentTag, _fvector vOrigin, _vector vDir, _float& fOutDist);
private:
	_uint							m_iNumGroups = {};
	list<class CGameObject*>*		m_pGameObjects = { nullptr };

public:
	static CCollider_Manager* Create(_uint iNumGroups);
	virtual void Free() override;
};

NS_END