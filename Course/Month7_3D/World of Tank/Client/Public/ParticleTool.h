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
	_int			m_iCurrentInstance = { 10 };
	const _int		m_iMaxInstance = 10000;
	CVIBuffer_Point_Instance* m_VIBuffer = { nullptr };

public:
	static CParticleTool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END