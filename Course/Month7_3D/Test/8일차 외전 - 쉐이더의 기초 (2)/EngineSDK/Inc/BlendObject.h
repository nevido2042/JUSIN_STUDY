#pragma once

#include "GameObject.h"

/* ������ �ʿ��� ��ü���� ī�޶�� ������ ���̸� ����Ͽ��� �����Ѵ�. */
/* ���̸� ���� ����. */

BEGIN(Engine)

class ENGINE_DLL CBlendObject abstract : public CGameObject
{
protected:
	CBlendObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CBlendObject(const CBlendObject& Prototype);
	virtual ~CBlendObject() = default;

public:
	_float Get_Depth() const {
		return m_fDepth;
	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	/* ī�޶�κ����� ���̸� ����Ѵ�.  */
	_float				m_fDepth = {};

protected:
	void Compute_CameraDepth(const _float3& vPosition);
	
public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free();
};

END