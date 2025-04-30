#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

#include "Network.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Cube;
END

BEGIN(Client)

class CCube final : public CGameObject
{
public:
	typedef struct tagCubeDesc
	{
		int iID{ 0 };
		_float3 Postion{};
	}CUBE_DESC;

private:
	CCube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCube(const CCube& Prototype);
	virtual ~CCube() = default;

public:
	virtual HRESULT Initialize_Prototype()override;
	virtual HRESULT Initialize(void* pArg)override;
	virtual void Priority_Update(_float fTimeDelta)override;
	virtual void Update(_float fTimeDelta)override;
	virtual void Late_Update(_float fTimeDelta)override;
	virtual HRESULT Render()override;

public:
	int Get_ID();
	CTransform* Get_TransformCom(); //뭔가 아닌것 같지만 임시로
	void Set_Move(bool _bool);
	void Set_MoveDirection(_float3& vMoveDir);
private:
	CNetwork*	m_pNetwork{ nullptr };
	int			m_iMyID;
	_float3		m_OldPosition;
	bool		m_IsMove{ false };
	_float3		m_vMoveDir{};


private:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Cube* m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Components();


public:
	static CCube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free();
};

END