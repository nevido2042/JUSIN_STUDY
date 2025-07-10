#pragma once

#include "Client_Defines.h"
#include "Building.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
NS_END

NS_BEGIN(Client)

class CKitCrashFactoryWall01A final : public CBuilding
{
private:
	CKitCrashFactoryWall01A(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CKitCrashFactoryWall01A(const CKitCrashFactoryWall01A& Prototype);
	virtual ~CKitCrashFactoryWall01A() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_Shadow() override;

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CKitCrashFactoryWall01A* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END