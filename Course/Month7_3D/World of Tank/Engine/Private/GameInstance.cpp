#include "GameInstance.h"

#include "Renderer.h"
#include "Level_Manager.h"
#include "Timer_Manager.h"
#include "Graphic_Device.h"
#include "Object_Manager.h"
#include "Prototype_Manager.h"
#include "Sound_Device.h"
#include "Input_Device.h"
#include "PipeLine.h"
#include "Network.h"
#include "Picking.h"
#include "Frustum.h"
#include "Light_Manager.h"
#include "Font_Manager.h"


_uint g_iWinSizeX;
_uint g_iWinSizeY;
_bool g_bWindowResizeRequired = false;
HWND g_hWnd;

IMPLEMENT_SINGLETON(CGameInstance);

CGameInstance::CGameInstance()
{
}

HRESULT CGameInstance::Initialize_Engine(const ENGINE_DESC& EngineDesc, _Out_ ID3D11Device** ppDeviceOut, ID3D11DeviceContext** ppContextOut)
{
	m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.isWindowed, EngineDesc.iWinSizeX, EngineDesc.iWinSizeY, ppDeviceOut, ppContextOut);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pInputDevice = CInput_Device::Create(EngineDesc.hInst, EngineDesc.hWnd);
	if (nullptr == m_pInputDevice)
		return E_FAIL;

	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pPrototype_Manager = CPrototype_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pPrototype_Manager)
		return E_FAIL;

	m_pObject_Manager = CObject_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pRenderer = CRenderer::Create(*ppDeviceOut, *ppContextOut);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	m_pSound_Device = CSound_Device::Create();
	if (nullptr == m_pSound_Device)
		return E_FAIL;

	m_pPipeLine = CPipeLine::Create();
	if (nullptr == m_pPipeLine)
		return E_FAIL;

	m_pNetwork = CNetwork::Create();
	if (nullptr == m_pNetwork)
		return E_FAIL;

	m_pPicking = CPicking::Create(*ppDeviceOut, *ppContextOut, EngineDesc.hWnd/*, EngineDesc.iWinSizeX, EngineDesc.iWinSizeY*/);
	if (nullptr == m_pPicking)
		return E_FAIL;

	m_pFrustum = CFrustum::Create();
	if (nullptr == m_pFrustum)
		return E_FAIL;

	m_pLight_Manager = CLight_Manager::Create();
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	m_pFont_Manager = CFont_Manager::Create(*ppDeviceOut, *ppContextOut);
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta, _uint iWinSizeX, _uint iWinSizeY)
{
	if (g_bWindowResizeRequired)
		m_pGraphic_Device->Resize(g_iWinSizeX, g_iWinSizeY);


	if (GetForegroundWindow() == g_hWnd)
	{
		m_pInputDevice->Update();

		m_pSound_Device->MuteSound(false);
		m_pSound_Device->Update();
	}
	else
		m_pSound_Device->MuteSound(true);

	m_pNetwork->Priority_Update(fTimeDelta);
	m_pObject_Manager->Priority_Update(fTimeDelta);
	m_pPipeLine->Update();

	m_pFrustum->Update();
	m_pPicking->Update(iWinSizeX, iWinSizeY);
	m_pNetwork->Update(fTimeDelta);
	m_pObject_Manager->Update(fTimeDelta);	

	m_pNetwork->Late_Update(fTimeDelta);
	m_pObject_Manager->Late_Update(fTimeDelta);

	m_pLevel_Manager->Update(fTimeDelta);

	g_bWindowResizeRequired = false;
}

HRESULT CGameInstance::Begin_Draw()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pGraphic_Device->Clear_BackBuffer_View(_float4(0.f, 0.0f, 1.f, 1.f));
	m_pGraphic_Device->Clear_DepthStencil_View();

	return S_OK;
}

HRESULT CGameInstance::Draw()
{
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pRenderer->Draw();

	m_pLevel_Manager->Render();

	return S_OK;
}

HRESULT CGameInstance::End_Draw()
{
	m_pGraphic_Device->Present();

	return S_OK;
}

void CGameInstance::Clear(_uint iLevelIndex)
{
	/* 특정 레벨의 자원을 삭제한다. */
	
	/* 특정 레벨의 객체을 삭제한다. */
	m_pObject_Manager->Clear(iLevelIndex);

	/* 특정 레벨의 원형객을 삭제한다. */
	m_pPrototype_Manager->Clear(iLevelIndex);
}

_float CGameInstance::Compute_Random_Normal()
{
	return rand() / static_cast<_float>(RAND_MAX);	
}

_float CGameInstance::Compute_Random(_float fMin, _float fMax)
{
	return fMin + (fMax - fMin) * Compute_Random_Normal();	
}

#pragma region Graphic_Device
HRESULT CGameInstance::Resize(_uint iWinResizeX, _uint iWinResizeY)
{
	if (m_pGraphic_Device)
		m_pGraphic_Device->Resize(iWinResizeX, iWinResizeY);

	return S_OK;
}
#pragma endregion

#pragma region LEVEL_MANAGER

_uint CGameInstance::Get_NewLevel_Index() const
{
	return m_pLevel_Manager->Get_NewLevel_Index();
}

HRESULT CGameInstance::Change_Level(_uint iNewLevelIndex)
{
	return m_pLevel_Manager->Change_Level(iNewLevelIndex);
}

HRESULT CGameInstance::Change_Level(_uint iLevelIndex, CLevel* pNewLevel)
{
	return m_pLevel_Manager->Change_Level(iLevelIndex, pNewLevel);
}
#pragma endregion

#pragma region PROTOTYPE_MANAGER

HRESULT CGameInstance::Add_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, CBase* pPrototype)
{
	return m_pPrototype_Manager->Add_Prototype(iPrototypeLevelIndex, strPrototypeTag, pPrototype);
}

CBase* CGameInstance::Clone_Prototype(PROTOTYPE ePrototypeType, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	return m_pPrototype_Manager->Clone_Prototype(ePrototypeType, iPrototypeLevelIndex, strPrototypeTag, pArg);
}
const PROTOTYPES& CGameInstance::Get_Prototypes(_uint iPrototypeLevelIndex) const
{
	return m_pPrototype_Manager->Get_Prototypes(iPrototypeLevelIndex);
}
#pragma endregion

#pragma region OBJECT_MANAGER
HRESULT CGameInstance::Add_GameObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg)
{
	return m_pObject_Manager->Add_GameObject(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, strLayerTag, pArg);
}

CGameObject* CGameInstance::Get_GameObject(_uint iLevelIndex, const _wstring& strLayerTag, _uint iIndex)
{
	return m_pObject_Manager->Get_GameObject(iLevelIndex, strLayerTag, iIndex);
}

CGameObject* CGameInstance::Get_Last_GameObject(_uint iLevelIndex, const _wstring& strLayerTag)
{
	return m_pObject_Manager->Get_Last_GameObject(iLevelIndex, strLayerTag);
}

CComponent* CGameInstance::Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex)
{
	return m_pObject_Manager->Get_Component(iLevelIndex, strLayerTag, strComponentTag, iIndex);	
}

const map<const _wstring, class CLayer*>& CGameInstance::Get_Layers(_uint iLevelIndex) const
{
	return m_pObject_Manager->Get_Layers(iLevelIndex);
}

CLayer* CGameInstance::Find_Layer(_uint iLevelIndex, const _wstring& strLayerTag)
{
	return m_pObject_Manager->Find_Layer(iLevelIndex, strLayerTag);
}

#pragma endregion

#pragma region RENDERER

HRESULT CGameInstance::Add_RenderGroup(RENDERGROUP eRenderGroup, CGameObject* pRenderObject)
{
	return m_pRenderer->Add_RenderGroup(eRenderGroup, pRenderObject);
}

#pragma endregion

#pragma region TIMER_MANAGER

_float CGameInstance::Get_TimeDelta(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Get_TimeDelta(strTimerTag);
}

HRESULT CGameInstance::Add_Timer(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Add_Timer(strTimerTag);
}

void CGameInstance::Update_Timer(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Update(strTimerTag);
}

#pragma endregion

#pragma region PICKING
void CGameInstance::Transform_Picking_ToLocalSpace(const _matrix& WorldMatrixInverse)
{
	m_pPicking->Transform_ToLocalSpace(WorldMatrixInverse);
}
_bool CGameInstance::Picking_InWorld(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC)
{
	return m_pPicking->Picking_InWorld(vPickedPos, vPointA, vPointB, vPointC);
}
_bool CGameInstance::Picking_InLocal(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC)
{
	return m_pPicking->Picking_InLocal(vPickedPos, vPointA, vPointB, vPointC);
}

const _float3& CGameInstance::Get_MousePos()
{
	return m_pPicking->Get_MousePos();
}

const _float3& CGameInstance::Get_MouseRay()
{
	return m_pPicking->Get_MouseRay();
}

const _float3& CGameInstance::Get_ScreenCenterPos()
{
	return m_pPicking->Get_ScreenCenterPos();
}
const _float3& CGameInstance::Get_ScreenCenterRay()
{
	return m_pPicking->Get_ScreenCenterRay();
}

#pragma endregion

#pragma region Sound_Device
FORCEINLINE
HRESULT CGameInstance::LoadSound(const string& Path, _bool is3D, _bool loop, _bool stream, unordered_map<string, class CSound_Core*>* _Out_ pOut)
{
	return m_pSound_Device->LoadSound(Path, is3D, loop, stream, pOut);
}

FORCEINLINE
CSound_Core* CGameInstance::Get_Single_Sound(const string& strKey)
{
	return m_pSound_Device->Get_Single_Sound(strKey);
}

FORCEINLINE
void CGameInstance::Set_Listener_Position(const CTransform* pTransform, const _float3& vel)
{
	m_pSound_Device->Set_Listener_Position(pTransform, vel);
}

FORCEINLINE
void CGameInstance::Set_Master_Volume(_float volume)
{
	m_pSound_Device->Set_Master_Volume(volume);
}
#pragma endregion

void CGameInstance::MuteSound(_bool bMute)
{
	m_pSound_Device->MuteSound(bMute);
}

#pragma region INPUT_DEVICE

FORCEINLINE
_byte CGameInstance::Get_DIKState(_ubyte eKeyID)
{
	return m_pInputDevice->Get_DIKState(eKeyID);
}

FORCEINLINE
_byte CGameInstance::Get_DIMKeyState(DIMK eMouseKeyID)
{
	return m_pInputDevice->Get_DIMKeyState(eMouseKeyID);
}

FORCEINLINE
_int CGameInstance::Get_DIMMoveState(DIMM eMouseMoveID)
{
	return m_pInputDevice->Get_DIMMoveState(eMouseMoveID);
}

FORCEINLINE
_bool CGameInstance::Mouse_Down(_ubyte eKeyID)
{
	return m_pInputDevice->Mouse_Down(eKeyID);
}

FORCEINLINE
_bool CGameInstance::Mouse_Pressing(_ubyte eKeyID)
{
	return m_pInputDevice->Mouse_Pressing(eKeyID);
}

FORCEINLINE
_bool CGameInstance::Mouse_Up(_ubyte eKeyID)
{
	return m_pInputDevice->Mouse_Up(eKeyID);
}


FORCEINLINE
_bool CGameInstance::Key_Pressing(_ubyte eKeyID)
{
	return m_pInputDevice->Key_Pressing(eKeyID);
}

FORCEINLINE
_bool CGameInstance::Key_Up(_ubyte eKeyID)
{
	return m_pInputDevice->Key_Up(eKeyID);
}

FORCEINLINE
_bool CGameInstance::Key_Down(_ubyte eKeyID)
{
 	return m_pInputDevice->Key_Down(eKeyID);
}
#pragma endregion

#pragma region PIPELINE

void CGameInstance::Set_Transform(D3DTS eState, _fmatrix TransformMatrix)
{
	m_pPipeLine->Set_Transform(eState, TransformMatrix);
}

const _float4x4* CGameInstance::Get_Transform_Float4x4(D3DTS eState) const
{
	return m_pPipeLine->Get_Transform_Float4x4(eState);
}

_matrix CGameInstance::Get_Transform_Matrix(D3DTS eState) const
{
	return m_pPipeLine->Get_Transform_Matrix(eState);
}

const _float4* CGameInstance::Get_CamPosition() const
{
	return m_pPipeLine->Get_CamPosition();
}

void	CGameInstance::Set_Fov(_float fFov)
{
	m_pPipeLine->Set_Fov(fFov);
}
_float	CGameInstance::Get_Fov() const
{
	return m_pPipeLine->Get_Fov();
}

#pragma endregion

#pragma region Network
HRESULT CGameInstance::Login()
{
	return m_pNetwork->Login();
}

void CGameInstance::Set_ID(_uint iID)
{
	m_pNetwork->Set_ID(iID);
}

_uint CGameInstance::Get_ID()
{
	return m_pNetwork->Get_ID();
}

HRESULT CGameInstance::Send_Packet(_uint iPacketType, void* pArg)
{
	return m_pNetwork->Send_Packet(iPacketType, pArg);
}

HRESULT CGameInstance::Define_Packet(_uint iPacketType, function<void(void*)> pFunction)
{
	return m_pNetwork->Define_Packet(iPacketType, pFunction);
}

NETWORK_STATUS CGameInstance::Get_Network_Status()
{
	return m_pNetwork->Get_Network_Status();
}

void CGameInstance::Set_Network_Status(NETWORK_STATUS eStatus)
{
	return m_pNetwork->Set_Network_Status(eStatus);
}

HRESULT CGameInstance::Clear_Packet()
{
	return m_pNetwork->Clear_Packet();
}

HRESULT CGameInstance::Input_Data(_byte* pByte, _int iSize)
{
	return m_pNetwork->Input_Data(pByte, iSize);
}

HRESULT CGameInstance::Output_Data(_byte* pByte, _int iSize)
{
	return m_pNetwork->Output_Data(pByte, iSize);
}

HRESULT CGameInstance::Update_Header()
{
	return m_pNetwork->Update_Header();
}
#pragma endregion

#pragma region Frustum
_bool CGameInstance::Is_In_Frustum(_fvector vPos)
{
	return m_pFrustum->Is_In_Frustum(vPos);
}

_bool CGameInstance::Is_In_Frustum(_fvector vPos, _float fRadius)
{
	return m_pFrustum->Is_In_Frustum(vPos, fRadius);
}
#pragma endregion

#pragma region Light_Manager
const LIGHT_DESC* CGameInstance::Get_Light(_uint iIndex)
{
	return m_pLight_Manager->Get_Light(iIndex);
}

HRESULT CGameInstance::Add_Light(const LIGHT_DESC& LightDesc)
{
	return m_pLight_Manager->Add_Light(LightDesc);
}
#pragma endregion

#pragma region FONT_MANAGER
HRESULT CGameInstance::Add_Font(const _wstring& strFontTag, const _tchar* pFontFilePath)
{
	return m_pFont_Manager->Add_Font(strFontTag, pFontFilePath);
}
void CGameInstance::Draw_Font(const _wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _fvector vColor, _float fRotation, const _float2& vOrigin, _float fScale)
{
	m_pFont_Manager->Draw(strFontTag, pText, vPosition, vColor, fRotation, vOrigin, fScale);
}
#pragma endregion


void CGameInstance::Release_Engine()
{
	if (0 != Safe_Release(m_pTimer_Manager))
		MSG_BOX("Failed to Release : m_pTimer_Manager");

	if (0 != Safe_Release(m_pRenderer))
		MSG_BOX("Failed to Release : m_pRenderer");

	if (0 != Safe_Release(m_pObject_Manager))
		MSG_BOX("Failed to Release : m_pObject_Manager");

	if (0 != Safe_Release(m_pPrototype_Manager))
		MSG_BOX("Failed to Release : m_pPrototype_Manager");

	if (0 != Safe_Release(m_pLevel_Manager))
		MSG_BOX("Failed to Release : m_pLevel_Manager");

	if (0 != Safe_Release(m_pSound_Device))
		MSG_BOX("Failed to Release : m_pSound_Device");

	if (0 != Safe_Release(m_pInputDevice))
		MSG_BOX("Failed to Release : m_pInputDevice");

	if (0 != Safe_Release(m_pPipeLine))
		MSG_BOX("Failed to Release : m_pPipeLine");

	if (0 != Safe_Release(m_pNetwork))
		MSG_BOX("Failed to Release : m_pNetwork");

	if (0 != Safe_Release(m_pPicking))
		MSG_BOX("Failed to Release : m_pPicking");

	if (0 != Safe_Release(m_pFrustum))
		MSG_BOX("Failed to Release : m_pFrustum");

	if (0 != Safe_Release(m_pLight_Manager))
		MSG_BOX("Failed to Release : m_pLight_Manager");

	if (0 != Safe_Release(m_pFont_Manager))
		MSG_BOX("Failed to Release : m_pFont_Manager");

	if (0 != Safe_Release(m_pGraphic_Device))
		MSG_BOX("Failed to Release : m_pGraphic_Device");

	Destroy_Instance();
}

void CGameInstance::Free()
{
	__super::Free();



	
}
