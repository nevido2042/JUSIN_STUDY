#pragma once
#include "Client_Defines.h"
#include "Tool.h"

NS_BEGIN(Client)

class CEngin_Sound_Tool final : public CTool
{
protected:
	CEngin_Sound_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CEngin_Sound_Tool() = default;

public:
	virtual HRESULT Initialize();
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	static CEngin_Sound_Tool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

NS_END