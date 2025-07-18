﻿#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Terrain;
NS_END

NS_BEGIN(Client)

class CTerrain final : public CGameObject
{
public:
	enum TEXTURE { TEXTURE_DIFFUSE, TEXTURE_MASK, TEXTURE_END };
private:
	CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTerrain(const CTerrain& Prototype);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	_bool Pick(_fvector vOrigin, _fvector vRayDir, _float& fOutDist);

public:
	_float3						Get_PickedPos() const { return m_vPickedPos; }

private:
	HRESULT Picking_Mouse();

private:
	_float3				m_vPickedPos = { 0.f, 0.f, 0.f };

private:
	CShader*			m_pShaderCom = { nullptr };
	CTexture*			m_pTextureCom[TEXTURE_END] = { nullptr };
	CVIBuffer_Terrain*	m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END