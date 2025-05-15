#pragma once

#include "Transform.h"

NS_BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	typedef struct tagGameObjectDesc : public CTransform::TRANSFORM_DESC
	{
		_tchar		szName[MAX_PATH];
	}GAMEOBJECT_DESC;

protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& Prototype);
	virtual ~CGameObject() = default;

public:
	CComponent* Get_Component(const _wstring& strComponentTag);

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	_bool	Get_IsDestroyed() { return m_bDestroyed; }
	void	Destroy() { m_bDestroyed = true; }

	class CTransform* const Get_Transform() { return m_pTransformCom; }

protected:
	_bool m_bCloned = { false };
	_bool m_bDestroyed = { false };

protected:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };

	class CGameInstance*		m_pGameInstance = { nullptr };

protected:
	_tchar										m_szName[MAX_PATH] = {};
	map<const _wstring, class CComponent*>		m_Components;
	class CTransform*							m_pTransformCom = { nullptr };

protected:
	HRESULT Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, CComponent** ppOut, void* pArg = nullptr);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END