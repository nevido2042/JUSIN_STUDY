#pragma once

/* 엔진과 클라이언트 간의 링크의 역활을 수행한다. */
/* 엔진 내에 존재하는 유일한 싱글톤 클래스이다. */
/* 엔진 개발자가 클라개밫자에게 보여주고싶은 함수를 ... */
#include "Prototype_Manager.h"

BEGIN(Engine)


class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance);

private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize_Engine(const ENGINE_DESC& EngineDesc, _Out_ ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppContextOut);
	void Update_Engine(_float fTimeDelta);
	void Clear(_uint iLevelIndex);
	HRESULT Begin_Draw();
	HRESULT Draw();
	HRESULT End_Draw();

	_float Compute_Random_Normal();
	_float Compute_Random(_float fMin, _float fMax);

#pragma region LEVEL_MANAGER
public:
	HRESULT Change_Level(_uint iLevelIndex, class CLevel* pNewLevel);
#pragma endregion

#pragma region PROTOTYPE_MANAGER
	HRESULT Add_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);
	CBase* Clone_Prototype(PROTOTYPE ePrototypeType, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
#pragma endregion

#pragma region OBJECT_MANAGER
	HRESULT Add_GameObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr);
	class CGameObject* Get_GameObject(_uint iLevelIndex, const _wstring& strLayerTag, _uint iIndex = 0);
	CComponent* Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex = 0);
#pragma endregion

#pragma region RENDERER
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
#pragma endregion

#pragma region TIMER_MANAGER
	_float Get_TimeDelta(const _wstring& strTimerTag);
	HRESULT Add_Timer(const _wstring& strTimerTag);
	void Update_Timer(const _wstring& strTimerTag);
#pragma endregion

#pragma region SOUND_DEVICE
	// Out에 nullptr 넣으면 싱글사운드로 등록됨
	HRESULT LoadSound(const string& Path, _bool is3D = FALSE, _bool loop = FALSE, _bool stream = FALSE, unordered_map<string, class CSound_Core*>* _Out_ pOut = nullptr);
	// 난 에드래퍼 올릴거다 분명히 말했다 래퍼 올린다 릴리즈 시키시오
	class CSound_Core* Get_Single_Sound(const string& strKey);
	// 리스너 포지션 제어(업룩포 요구해서 트랜스폼 받았음)
	void Set_Listener_Position(const class CTransform* pTransform, const _float3& vel);
	// 전체 볼륨 제어
	void Set_Master_Volume(_float volume);
#pragma endregion

#pragma region INPUT_DEVICE
	_byte Get_DIKState(_ubyte eKeyID);
	_byte Get_DIMKeyState(DIMK eMouseKeyID);
	_long Get_DIMMoveState(DIMM eMouseMoveID);

	_bool Mouse_Down(_ubyte eKeyID);
	_bool Mouse_Pressing(_ubyte eKeyID);
	_bool Mouse_Up(_ubyte eKeyID);

	_bool		Key_Pressing(_ubyte eKeyID);
	_bool		Key_Up(_ubyte eKeyID);
	_bool		Key_Down(_ubyte eKeyID);
#pragma endregion

//
//#pragma region PICKING
//	void Transform_Picking_ToLocalSpace(const _float4x4& WorldMatrixInverse);
//	_bool Picking_InWorld(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC);
//	_bool Picking_InLocal(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC);
//#pragma endregion

private:
	class CGraphic_Device*		m_pGraphic_Device = { nullptr };
	class CLevel_Manager*		m_pLevel_Manager = { nullptr };
	class CPrototype_Manager*	m_pPrototype_Manager = { nullptr };
	class CObject_Manager*		m_pObject_Manager = { nullptr };
	class CRenderer*			m_pRenderer = { nullptr };
	class CTimer_Manager*		m_pTimer_Manager = { nullptr };
	class CSound_Device*		m_pSound_Device = { nullptr };
	class CInput_Device* 		m_pInputDevice = { nullptr };
	class CKey_Manager*			m_pKeyManager = { nullptr };

	// class CPicking*				m_pPicking = { nullptr };

public:
	void Release_Engine();
	virtual void Free() override;
};

END