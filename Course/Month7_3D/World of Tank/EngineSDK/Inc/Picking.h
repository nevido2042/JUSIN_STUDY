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

	/* 마우스의 윈도우상의 위치를 구하고 */
	/* 뷰스페이스 상에서 마우스 레이로 변형하여 */
	/* 월드까지 변환시켜서 보관하자. */
	void Update(_uint iWinSizeX, _uint iWinSizeY);

	/* 기존의 그냥 피킹함수를 로컬과 월드에서 각각 해주기 위해, InWorld와 InLocal로 함수를 나눠줬다 */
	_bool Picking_InWorld(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC);
	_bool Picking_InLocal(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC);

	/* 마우스의 방향과 좌표를 월드에서 로컬로 보내주기 위한 함수 */
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

	/* 로컬좌표로 옮겨온 마우스의 방향과 위치를 저장하기 위한 변수 */
	_float3			m_vLocalMouseRay = {};
	_float3			m_vLocalMousePos = {};

public:
	static CPicking* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd/*, _uint iWinSizeX, _uint iWinSizeY*/);
	virtual void Free() override;
};

NS_END