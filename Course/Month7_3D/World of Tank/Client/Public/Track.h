#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
NS_END

NS_BEGIN(Client)

class CTrack abstract : public CGameObject
{
protected:
	CTrack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTrack(const CTrack& Prototype);
	virtual ~CTrack() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_Outline() override;
	virtual HRESULT Render_Shadow() override;
	virtual void On_RaycastHit(CGameObject* pOther) override;

public:
	void Set_Speed(_float fSpeed) { m_fSpeed = fSpeed; }

protected:
	_float			m_fSpeed = {};
	_float			m_fUVScrollY = {};

protected:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };

protected:
	HRESULT Bind_ShaderResources();

public:
	virtual void Free() override;

};

NS_END