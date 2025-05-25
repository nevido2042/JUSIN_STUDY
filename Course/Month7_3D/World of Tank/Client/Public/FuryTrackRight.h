#pragma once

#include "Client_Defines.h"
#include "GameObject.h"
#include "Track.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)

class CFuryTrackRight final : public CTrack
{
private:
	CFuryTrackRight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFuryTrackRight(const CFuryTrackRight& Prototype);
	virtual ~CFuryTrackRight() = default;

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
	static CFuryTrackRight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END