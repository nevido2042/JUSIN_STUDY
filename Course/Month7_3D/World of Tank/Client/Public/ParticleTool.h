#pragma once

#include "Client_Defines.h"
#include "Tool.h"


NS_BEGIN(Engine)
class CVIBuffer_Point_Instance;
NS_END

NS_BEGIN(Client)

class CParticleTool final : public CTool
{
private:
	CParticleTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CParticleTool(const CParticleTool& Prototype);
	virtual ~CParticleTool() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	wstring OpenFileDialog();
	HRESULT Load_Texture(const wstring& strPrototypeTag, const wstring& pTextureFilePath);
	HRESULT Change_Texture(const wstring& strPrototypeTag);
private:
	_int			m_iNumInstances = { 10 };
	_float3			m_vRange = { 1.f, 1.f, 1.f };
	_float3			m_vPivot = { 0.f, 0.f, 0.f };
	_float2			m_vSize = { 1.f, 3.f };
	_float2			m_vLifeTime = { 1.f, 3.f };
	_float2			m_vSpeed = { 1.f, 3.f };
	_bool			m_bLoop = { true };
	EMISSION_SHAPE	m_eEmissionShape = { EMISSION_SHAPE::END };
	static _uint	m_strPrototypeTag_ID;
private:
	CVIBuffer_Point_Instance* m_VIBuffer = { nullptr };
	class CSmoke*	m_pSmoke = { nullptr };

public:
	static CParticleTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END