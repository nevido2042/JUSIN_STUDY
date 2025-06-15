#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CCollider;
NS_END

NS_BEGIN(Client)

class CInvisibleWall final : public CGameObject
{
public:
	typedef struct tagInvisibleWallDesc : public GAMEOBJECT_DESC
	{
		_float3 vExtents = {};
	}INVISIBLE_WALL_DESC;

private:
	CInvisibleWall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CInvisibleWall(const CInvisibleWall& Prototype);
	virtual ~CInvisibleWall() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CCollider* m_pColliderCom = { nullptr };
private:
	HRESULT Ready_Components(void* pArg);

public:
	static CInvisibleWall* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END