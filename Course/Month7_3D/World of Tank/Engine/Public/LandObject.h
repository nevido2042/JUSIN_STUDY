#pragma once

#include "GameObject.h"

NS_BEGIN(Engine)

class ENGINE_DLL CLandObject abstract : public CGameObject
{
public:
	typedef struct tagLandObjectDesc : public GAMEOBJECT_DESC
	{
		_uint		iLevelIndex;
		_wstring	strLayerTag;
		_uint		iIndex;
		_wstring	strComponentTag;
	}LANDOBJECT_DESC;
protected:
	CLandObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLandObject(const CLandObject& Prototype);
	virtual ~CLandObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	class CVIBuffer* m_pTargetBuffer = { nullptr };

protected:
	void SetUp_Height_Normal(class CTransform* pTransform, _float fTimeDelta, _float fOffset = 0.f);
	void SetUp_Height(class CTransform* pTransform);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free();


};

NS_END