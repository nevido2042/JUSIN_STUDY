#pragma once

#include "Base.h"

#include "Engine_Enum.h"

NS_BEGIN(Engine)

class CInput_Device final : public CBase
{
private:
	CInput_Device();
	virtual ~CInput_Device() = default;

public:
	_byte Get_DIKState(_ubyte eKeyID) {
		return m_byKeyState[eKeyID];
	}

	_byte Get_DIMKeyState(DIMK eMouseKeyID) {
		return m_MouseState.rgbButtons[ENUM_CLASS(eMouseKeyID)];
	}

	_int Get_DIMMoveState(DIMM eMouseMoveID) {
		// return *(((_int*)&m_MouseState) + eMouseMoveID);
		return ((_int*)&m_MouseState)[ENUM_CLASS(eMouseMoveID)];
	}

	_bool Key_Down(_ubyte eKeyID)
	{
		return (m_byKeyState[eKeyID] & 0x80) && !(m_byPrevKeyState[eKeyID] & 0x80);
	}

	_bool Key_Pressing(_ubyte eKeyID)
	{
		return (m_byKeyState[eKeyID] & 0x80);
	}

	_bool Key_Up(_ubyte eKeyID)
	{
		return !(m_byKeyState[eKeyID] & 0x80) && (m_byPrevKeyState[eKeyID] & 0x80);
	}

	_bool Mouse_Down(_ubyte eKeyID)
	{
		return (m_MouseState.rgbButtons[eKeyID] & 0x80) && !(m_PrevMouseState.rgbButtons[eKeyID] & 0x80);
	}

	_bool Mouse_Pressing(_ubyte eKeyID)
	{
		return (m_MouseState.rgbButtons[eKeyID] & 0x80);
	}

	_bool Mouse_Up(_ubyte eKeyID)
	{
		return !(m_MouseState.rgbButtons[eKeyID] & 0x80) && (m_PrevMouseState.rgbButtons[eKeyID] & 0x80);
	}

public:
	HRESULT Initialize(HINSTANCE hInstance, HWND hWnd);
	void Update();

private:
	LPDIRECTINPUT8				m_pInputSDK = nullptr;
	LPDIRECTINPUTDEVICE8		m_pKeyboard = nullptr;
	LPDIRECTINPUTDEVICE8		m_pMouse = nullptr;

private:
	_byte						m_byKeyState[256] = { 0 };
	_byte						m_byPrevKeyState[256] = { 0 };
	DIMOUSESTATE				m_MouseState{};
	DIMOUSESTATE				m_PrevMouseState{};

public:
	static CInput_Device* Create(HINSTANCE hInstance, HWND hWnd);
	virtual void Free();
};

NS_END