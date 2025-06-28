#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Point_Instance;
NS_END

NS_BEGIN(Client)

class CSmoke final : public CGameObject
{
private:
	CSmoke(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSmoke(const CSmoke& Prototype);
	virtual ~CSmoke() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Reset_Smoke_Pivot();
	void Add_Smoke_Pivot(_float vSpeed);

	void Set_Emission(_bool bEmission) {
		m_bEmission = bEmission;
	}

	void Set_Loop(_bool bisLoop);

private:
	_bool	m_bEmission = { true };
private:
	_float3	m_InitPivot = {};
	_float3 m_OffsetPivot = {};
private:
	_float2	m_InitSpeed = {};

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Point_Instance* m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Components();

public:
	static CSmoke* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END