#pragma once

/* 엔진과 클라이언트 간의 링크의 역활을 수행한다. */
/* 엔진 내에 존재하는 유일한 싱글톤 클래스이다. */
/* 엔진 개발자가 클라개밫자에게 보여주고싶은 함수를 ... */
#include "Prototype_Manager.h"

NS_BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance);

private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize_Engine(const ENGINE_DESC& EngineDesc, _Out_ ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppContextOut);
	void Update_Engine(_float fTimeDelta, _uint iWinSizeX, _uint iWinSizeY);
	void Clear(_uint iLevelIndex);
	HRESULT Begin_Draw();
	HRESULT Draw();
	HRESULT End_Draw();

	_float Compute_Random_Normal();
	_float Compute_Random(_float fMin, _float fMax);

public:
#pragma region GRAPHIC_DEVICE
	HRESULT Resize(_uint iWinResizeX, _uint iWinResizeY);
#pragma endregion

#pragma region LEVEL_MANAGER
	_uint		Get_NewLevel_Index() const;
	//이걸로 레벨 변경하면 한프레임 파랗게 보였다가 로딩시작함, 하지만 게임오브젝트에서 호출해도 이터레이터 안터짐
	HRESULT		Change_Level(_uint iNewLevelIndex);
	HRESULT		Change_Level(_uint iLevelIndex, class CLevel* pNewLevel);
#pragma endregion

#pragma region PROTOTYPE_MANAGER
	HRESULT Add_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);
	CBase* Clone_Prototype(PROTOTYPE ePrototypeType, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
	const PROTOTYPES& Get_Prototypes(_uint iPrototypeLevelIndex) const;
#pragma endregion

#pragma region OBJECT_MANAGER
	HRESULT Add_GameObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr);
	class CGameObject* Get_GameObject(_uint iLevelIndex, const _wstring& strLayerTag, _uint iIndex = 0);
	class CGameObject* Get_Last_GameObject(_uint iLevelIndex, const _wstring& strLayerTag);
	CComponent* Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex = 0);
	const map<const _wstring, class CLayer*>& Get_Layers(_uint iLevelIndex) const;
	class CLayer* Find_Layer(_uint iLevelIndex, const _wstring& strLayerTag);
#pragma endregion

#pragma region RENDERER
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
#pragma endregion

#pragma region TIMER_MANAGER
	_float Get_TimeDelta(const _wstring& strTimerTag);
	HRESULT Add_Timer(const _wstring& strTimerTag);
	void Update_Timer(const _wstring& strTimerTag);
#pragma endregion

#pragma region PICKING
	void Transform_Picking_ToLocalSpace(const _matrix& WorldMatrixInverse);
	_bool Picking_InWorld(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC);
	_bool Picking_InLocal(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC);

	const _float3& Get_MousePos();
	const _float3& Get_MouseRay();

	const _float3& Get_ScreenCenterPos();
	const _float3& Get_ScreenCenterRay();
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
	//음소거
	void MuteSound(_bool bMute);
#pragma endregion

#pragma region INPUT_DEVICE
	_byte Get_DIKState(_ubyte eKeyID);
	_byte Get_DIMKeyState(DIMK eMouseKeyID);
	_int Get_DIMMoveState(DIMM eMouseMoveID);

	_bool Mouse_Down(_ubyte eKeyID);
	_bool Mouse_Pressing(_ubyte eKeyID);
	_bool Mouse_Up(_ubyte eKeyID);

	_bool		Key_Pressing(_ubyte eKeyID);
	_bool		Key_Up(_ubyte eKeyID);
	_bool		Key_Down(_ubyte eKeyID);
#pragma endregion

#pragma region 	PIPELINE
	void				Set_Transform(D3DTS eState, _fmatrix TransformMatrix);
	const _float4x4*	Get_Transform_Float4x4(D3DTS eState) const;
	_matrix			Get_Transform_Matrix(D3DTS eState) const;
	const _float4*	Get_CamPosition() const;
	void	Set_Fov(_float fFov);
	_float	Get_Fov() const;
#pragma endregion

#pragma region 	NETWORK
	HRESULT Login();
	void	Set_ID(_uint iID);
	_uint	Get_ID();
	HRESULT Send_Packet(_uint iPacketType, void* pArg = nullptr);
	HRESULT Define_Packet(_uint iPacketType, function<void(void*)> pFunction);
	NETWORK_STATUS	Get_Network_Status();
	void			Set_Network_Status(NETWORK_STATUS eStatus);

	HRESULT	Clear_Packet();
	HRESULT	Input_Data(_byte* pByte, _int iSize);
	HRESULT	Output_Data(_byte* pByte, _int iSize);
	HRESULT	Update_Header();
#pragma endregion

#pragma region 	Frustum
	_bool   Is_In_Frustum(_fvector vPos);
	_bool   Is_In_Frustum(_fvector vPos, _float fRadius);
#pragma endregion

#pragma region LIGHT_MANAGER
	const LIGHT_DESC* Get_Light(_uint iIndex);
	HRESULT Add_Light(const LIGHT_DESC& LightDesc);
#pragma endregion

#pragma region FONT_MANAGER
	HRESULT Add_Font(const _wstring& strFontTag, const _tchar* pFontFilePath);
	void Draw_Font(const _wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _fvector vColor = XMVectorSet(1.f, 1.f, 1.f, 1.f), _float fRotation = 0.f, const _float2& vOrigin = _float2(0.f, 0.f), _float fScale = 1.f);
#pragma endregion

#pragma region COLLIDER_MANAGER
	HRESULT				Add_CollisionGroup(_uint iGroupIndex, class CGameObject* pGameObject, wstring strComponentTag);
	HRESULT				Out_CollisionGroup(_uint iGroupIndex, class CGameObject* pGameObject);
	void				Check_Collision(_uint iGroupIndex, class CGameObject* pGameObject, wstring strComponentTag, wstring strOtherComponentTag);
	class CGameObject* Check_RaycastHit(_uint iGroupIndex, wstring strComponentTag, _fvector vOrigin, _fvector vDir, _float& fOutDist);
	class CGameObject*	Check_RaycastHit(_uint iGroupIndex, wstring strComponentTag, _fvector vOrigin, _fvector vDir, _float& fOutDist, _int iIgnoreID);
#pragma endregion

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
	class CPipeLine*			m_pPipeLine = { nullptr };
	class CNetwork*				m_pNetwork = { nullptr };
	class CPicking*				m_pPicking = { nullptr };
	class CFrustum*				m_pFrustum = { nullptr };
	class CLight_Manager*		m_pLight_Manager = { nullptr };
	class CFont_Manager*		m_pFont_Manager = { nullptr };
	class CCollider_Manager*	m_pCollider_Manager = { nullptr };

public:
	void Release_Engine();
	virtual void Free() override;
};

NS_END
