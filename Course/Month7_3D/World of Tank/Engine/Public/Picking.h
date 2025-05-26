#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CPicking final : public CBase
{
private:
	CPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CPicking() = default;

public:
	HRESULT Initialize(HWND hWnd/*, _uint iWinSizeX, _uint iWinSizeY*/);

	/* ���콺�� ��������� ��ġ�� ���ϰ� */
	/* �佺���̽� �󿡼� ���콺 ���̷� �����Ͽ� */
	/* ������� ��ȯ���Ѽ� ��������. */
	void Update(_uint iWinSizeX, _uint iWinSizeY);

	/* ������ �׳� ��ŷ�Լ��� ���ð� ���忡�� ���� ���ֱ� ����, InWorld�� InLocal�� �Լ��� ������� */
	_bool Picking_InWorld(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC);
	_bool Picking_InLocal(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC);

	/* ���콺�� ����� ��ǥ�� ���忡�� ���÷� �����ֱ� ���� �Լ� */
	void Transform_ToLocalSpace(const _matrix& WorldMatrixInverse);

	const _float3& Get_MousePos()	{ return m_vMousePos; }
	const _float3& Get_MouseRay()	{ return m_vMouseRay; }

	const _float3& Get_ScreenCenterPos() { return m_vScreenCenterPos; }
	const _float3& Get_ScreenCenterRay() { return m_vScreenCenterRay; }

private:
	void Update_Mouse();
	void Update_ScreenCenter();

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	class CGameInstance*	m_pGameInstance = { nullptr };

private:
	HWND			m_hWnd = {};
	//_uint			m_iWinSizeX{}, m_iWinSizeY{};
	
	_float3			m_vMouseRay = {};
	_float3			m_vMousePos = {};

	_float3			m_vScreenCenterRay = {};
	_float3			m_vScreenCenterPos = {};

	/* ������ǥ�� �Űܿ� ���콺�� ����� ��ġ�� �����ϱ� ���� ���� */
	_float3			m_vLocalMouseRay = {};
	_float3			m_vLocalMousePos = {};

public:
	static CPicking* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd/*, _uint iWinSizeX, _uint iWinSizeY*/);
	virtual void Free() override;
};

NS_END