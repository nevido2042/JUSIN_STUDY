#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class CVIBuffer;
NS_END

NS_BEGIN(Client)

class CMissile final : public CGameObject
{
private:
	CMissile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMissile(const CMissile& Prototype);
	virtual ~CMissile() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	class CVIBuffer* m_pTargetBuffer = { nullptr };

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CMissile* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END