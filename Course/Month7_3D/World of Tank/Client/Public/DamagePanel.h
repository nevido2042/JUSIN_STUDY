#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
class CSoundController;
NS_END

NS_BEGIN(Client)

class CDamagePanel final : public CUIObject
{
private:
	CDamagePanel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDamagePanel(const CDamagePanel& Prototype);
	virtual ~CDamagePanel() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Play_Voice_StartBattle();
public:
	void Play_Voice_ModuleState(MODULE eModule, MODULE_STATE eState);
public:
	void Play_Voice_Destroyed();
public:
	void Play_Sound_TakeDamage();
public:
	void Repair_All();

private:
	CShader*			m_pShaderCom = { nullptr };
	CTexture*			m_pTextureCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	CSoundController*	m_pSoundCom_Voice = { nullptr };
	CSoundController*	m_pSoundCom_TankSound2D = { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();

public:
	static CDamagePanel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END