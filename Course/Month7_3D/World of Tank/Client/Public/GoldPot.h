#pragma once
#include "3DCustom.h"
#include "Client_Defines.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)

class CGoldPot final : public C3DCustom
{
private:
	CGoldPot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGoldPot(const CGoldPot& Prototype);
	virtual ~CGoldPot() = default;

public:
	virtual HRESULT Initialize_Prototype() { return S_OK; };
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	//너무 작아서 안보이는 듯?
	//virtual HRESULT Render_Shadow() override;

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();

public:
	static CGoldPot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
