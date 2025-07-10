#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)

class C3DCustom abstract : public CGameObject
{
protected:
	C3DCustom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	C3DCustom(const C3DCustom& Prototype);
	virtual ~C3DCustom() = default;

public:
	virtual HRESULT Initialize_Prototype() { return S_OK; };
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_Shadow() override;
protected:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
protected:
	HRESULT Bind_ShaderResources();

	virtual void Free() override;
};

NS_END
