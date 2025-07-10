#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
class CSoundController;
NS_END

NS_BEGIN(Client)

class CPolishSignboards final : public CGameObject
{
private:
	CPolishSignboards(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPolishSignboards(const CPolishSignboards& Prototype);
	virtual ~CPolishSignboards() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_Shadow() override;
	virtual void    On_Collision_Stay(CGameObject* pOther, _fvector vNormal) override;

private:
	_bool		m_bIsFall = { false };
	_float3		m_vAxis = {};
	_bool		m_bFallComplete = { false };

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };
	CSoundController* m_pSoundCom = { nullptr };
private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CPolishSignboards* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END