#pragma once

#include "Client_Defines.h"
#include "Track.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)

class CTigerTrackLeft final : public CTrack
{
private:
	CTigerTrackLeft(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTigerTrackLeft(const CTigerTrackLeft& Prototype);
	virtual ~CTigerTrackLeft() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components();

public:
	static CTigerTrackLeft* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END