#pragma once

#include "Client_Defines.h"
#include "LandObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
NS_END

NS_BEGIN(Client)

class CBuilding abstract : public CLandObject
{
protected:
	CBuilding(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBuilding(const CBuilding& Prototype);
	virtual ~CBuilding() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Shadow() override;


private:
	virtual void OnGround(_float fTimeDelta);

protected:
	_float3 m_vLocalPoint[4] = {};

protected:
	CCollider* m_pColliderCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
protected:
	HRESULT Bind_ShaderResources();
public:
	virtual void Free() override;

};

NS_END