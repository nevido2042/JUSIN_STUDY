#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_VolumeMesh;
NS_END

NS_BEGIN(Client)

class CTrackDecal final : public CGameObject
{
public:
	typedef struct tagTrackDecalDesc : public GAMEOBJECT_DESC
	{
		_float3 vRot = {};
	}TRACKDECAL_DESC;

private:
	CTrackDecal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTrackDecal(const CTrackDecal& Prototype);
	virtual ~CTrackDecal() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	_float m_fAccTime = { 0.f };

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_VolumeMesh* m_pVIBufferCom = { nullptr };
private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CTrackDecal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END