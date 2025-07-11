#pragma once

#include "GameObject.h"

/* CUIObject_2D */
/* CUIObject_3D */

NS_BEGIN(Engine)

class ENGINE_DLL CUIObject abstract : public CGameObject
{
public:
	typedef struct tagUIObjectDesc : public CGameObject::GAMEOBJECT_DESC
	{
		_float			fX, fY, fDepth, fSizeX, fSizeY;
		_bool			bIsChild = { false };
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

public:
	HRESULT Resize(_uint iNewWidth, _uint iNewHeight);

protected:
	_bool isPick(HWND hWnd);

protected:
	/* 뷰포트 상의 유아이의 중심위치 fX, fY, 사이즈 fSiuzeX, fSizeY */
	_float			m_fX{}, m_fY{}, m_fDepth{}, m_fSizeX{}, m_fSizeY{};

	/* 직교 투영을 위한 행렬. */
	_float4x4		m_ViewMatrix{}, m_ProjMatrix{};

	_float m_fXRatio{};      // m_fX / OldWidth
	_float m_fYRatio{};      // m_fY / OldHeight
	_float m_fSizeXRatio{};  // m_fSizeX / OldWidth
	_float m_fSizeYRatio{};  // m_fSizeY / OldHeight

protected:
	_bool m_bIsChild = { false }; // 자식 유아이 여부

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END