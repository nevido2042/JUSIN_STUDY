#pragma once

#include "Transform.h"

NS_BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	typedef struct tagGameObjectDesc : public CTransform::TRANSFORM_DESC
	{
		_tchar				szName[MAX_PATH];
		const _float4x4*	pParentWorldMatrix = { nullptr };
		_bool				bActive = { true };
		_int				iID = { -1 };
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

	const _bool		Get_isActive() const		{ return m_bActive; }
	void			Set_Active(_bool bActive)	{ m_bActive = bActive; }

	const _bool		Get_isVisible() const { return m_bVisible; }
	void			Set_Visible(_bool bVisible) { m_bVisible = bVisible; }

	_int			Get_ID() const { return m_iID; }
	void			Set_ID(_int iID) { m_iID = iID; }

	class CTransform* const Get_Transform() { return m_pTransformCom; }

public:
	CGameObject*	Find_PartObject(const _wstring& strPartObjectTag);
	_matrix			Get_CombinedWorldMatrix() const { return XMLoadFloat4x4(&m_CombinedWorldMatrix); }

protected:
	_bool m_bCloned = { false };

protected:
	_bool m_bDestroyed = { false };
	_bool m_bActive = { true };
	_bool m_bVisible = { true };
protected:
	_int	m_iID = { -1 };

protected:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };

	class CGameInstance*		m_pGameInstance = { nullptr };

protected:
	_tchar										m_szName[MAX_PATH] = {};
	map<const _wstring, class CComponent*>		m_Components;
	class CTransform*							m_pTransformCom = { nullptr };

protected:
	map<const _wstring, class CGameObject*>				m_PartObjects;

protected:
	const _float4x4*		m_pParentWorldMatrix = { nullptr };
	_float4x4				m_CombinedWorldMatrix = {};

protected:
	CComponent*		Find_Part_Component(const _wstring& strPartObjectTag, const _wstring& strComponentTag);
	HRESULT			Add_PartObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strPartObjectTag, void* pArg = nullptr);

protected:
	HRESULT Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, CComponent** ppOut, void* pArg = nullptr);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END