#include "Key_Manager.h"

CKey_Manager::CKey_Manager()
{
}


_bool CKey_Manager::Key_Pressing(_int _Key)
{
	if (GetAsyncKeyState(_Key) & 0x8000)
		return true;

	return false;
}

_bool CKey_Manager::Key_Up(_int _Key)
{
	if (m_bKeyState[_Key] && !(GetAsyncKeyState(_Key) & 0x8000))
	{
		m_bKeyState[_Key] = !m_bKeyState[_Key];
		return true;
	}

	if (!m_bKeyState[_Key] && (GetAsyncKeyState(_Key) & 0x8000))
		m_bKeyState[_Key] = !m_bKeyState[_Key];


	return false;
}

_bool CKey_Manager::Key_Down(_int _Key)
{
	if (!m_bKeyState[_Key] && (GetAsyncKeyState(_Key) & 0x8000))
	{
		m_bKeyState[_Key] = !m_bKeyState[_Key];
		return true;
	}

	if (m_bKeyState[_Key] && !(GetAsyncKeyState(_Key) & 0x8000))
		m_bKeyState[_Key] = !m_bKeyState[_Key];

	return false;
}


CKey_Manager* CKey_Manager::Create()
{
	return new CKey_Manager;
}

void CKey_Manager::Free()
{
	__super::Free();
}
