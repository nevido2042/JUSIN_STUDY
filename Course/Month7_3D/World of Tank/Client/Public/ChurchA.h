#pragma once

#include "Client_Defines.h"
#include "Building.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
NS_END

NS_BEGIN(Client)

class CChurchA final : public CBuilding
{
private:
	CChurchA(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CChurchA(const CChurchA& Prototype);
	virtual ~CChurchA() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_Shadow() override;

//private:
//	CCollider* m_pColliderCom = { nullptr };
//	CShader* m_pShaderCom = { nullptr };
//	CModel* m_pModelCom = { nullptr };
private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
public:
	static CChurchA* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END