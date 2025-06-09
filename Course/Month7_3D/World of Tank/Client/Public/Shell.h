#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CCollider;
class CShader;
class CVIBuffer;
NS_END

NS_BEGIN(Client)

class CShell final : public CGameObject
{
public:
	typedef struct tagShellDesc : public GAMEOBJECT_DESC
	{
		_uint		iLevelIndex;
		_wstring	strLayerTag;
		_uint		iIndex;
		_wstring	strComponentTag;
	}SHELL_DESC;

private:
	CShell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShell(const CShell& Prototype);
	virtual ~CShell() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();
	virtual void    On_Collision_Stay(CGameObject* pOther) override;

private:
	CCollider* m_pColliderCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	class CVIBuffer* m_pTargetBuffer = { nullptr };
private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CShell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END