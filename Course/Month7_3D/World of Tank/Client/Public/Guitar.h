#pragma once
#include "3DCustom.h"
#include "Client_Defines.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)

class CGuitar final : public C3DCustom
{
private:
	CGuitar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGuitar(const CGuitar& Prototype);
	virtual ~CGuitar() = default;

public:
	virtual HRESULT Initialize_Prototype() { return S_OK; };
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();

public:
	static CGuitar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END
