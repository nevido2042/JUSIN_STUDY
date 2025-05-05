#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
class CSoundController;
END

BEGIN(Client)

class CStatusLight final : public CUIObject
{
public:
	typedef struct tagCStatusLightDesc : public CUIObject::UIOBJECT_DESC
	{

	}STATUSLIGHT_DESC;

private:
	CStatusLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStatusLight(const CStatusLight& Prototype);
	virtual ~CStatusLight() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	class CNetwork* m_pNetwork = { nullptr };

	NETWORK_STATUS m_eStatus = NETWORK_STATUS::NETWORK_STATUS_END;

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
private:
	HRESULT Ready_Components();

public:
	static CStatusLight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END