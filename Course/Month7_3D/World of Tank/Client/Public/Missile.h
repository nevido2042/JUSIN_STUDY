#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class CVIBuffer;
class CSoundController;
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
	_vector Bezier2(_float3 vStart, _float3 vControl, _float3 vEnd, _float fT);

private:
	_float3 vStartPos = {};
	_float fProgress = {0.f};

private:
	class CVIBuffer* m_pTargetBuffer = { nullptr };
private:
	class CCamera_TPS* m_pCameraTPS = { nullptr };

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CSoundController* m_pSoundCom = { nullptr };
private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();
	HRESULT Bind_ShaderResources();

public:
	static CMissile* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END