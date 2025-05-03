#pragma once

#include "GameObject.h"

/* CUIObject_2D */
/* CUIObject_3D */

BEGIN(Engine)

class ENGINE_DLL CUIObject abstract : public CGameObject
{
public:
	typedef struct tagUIObjectDesc : public CGameObject::GAMEOBJECT_DESC
	{
		_float			fX, fY, fSizeX, fSizeY;
	}UIOBJECT_DESC;
protected:
	CUIObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIObject(const CUIObject& Prototype);
	virtual ~CUIObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	/* ����Ʈ ���� �������� �߽���ġ fX, fY, ������ fSiuzeX, fSizeY */
	_float			m_fX{}, m_fY{}, m_fSizeX{}, m_fSizeY{};

	/* ���� ������ ���� ���. */
	_float4x4		m_ViewMatrix{}, m_ProjMatrix{};

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END