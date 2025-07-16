#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class CSoundController;
NS_END

NS_BEGIN(Client)

class CPlane final : public CGameObject
{
private:
	enum PLANE_DIR { PLANE_DIR_RIGHT = 0, PLANE_DIR_LEFT, PLANE_DIR_UP, PLANE_DIR_DOWN, PLANE_DIR_END };
	enum PLANE_SOUND { PLANE_SOUND_FLY, PLANE_SOUND_GUN, PLANE_SOUND_END };
private:
	CPlane(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlane(const CPlane& Prototype);
	virtual ~CPlane() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_Shadow() override;

private:
	_vector Bezier2(_float3 vStart, _float3 vControl, _float3 vEnd, _float fT);
	void Reset_Plane();
private:
	_float m_fAccTime = { 0.f };
	_float m_fWaitTime = { 1.f };

	_float3 m_vStartPos = {};
	_float3 m_vEndPos = {};
	_float m_fProgress = { 0.f };
	_float m_fControlHeight = { 0.f };

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CSoundController* m_pSoundCom[PLANE_SOUND_END] = {nullptr};
private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();
	HRESULT Bind_ShaderResources();

public:
	static CPlane* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END