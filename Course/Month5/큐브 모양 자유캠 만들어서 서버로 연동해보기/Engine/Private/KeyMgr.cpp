#include "KeyMgr.h"

CKeyMgr::CKeyMgr()
{
    ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
    ZeroMemory(m_bPrevKeyState, sizeof(m_bPrevKeyState));
}

// Ű ���¸� �� ������ ������Ʈ
void CKeyMgr::Update()
{
    for (int i = 0; i < 256; ++i) // 256���� Ű�ڵ� üũ
    {
        m_bPrevKeyState[i] = m_bKeyState[i];  // ���� Ű ���� ����
        m_bKeyState[i] = (GetAsyncKeyState(i) & 0x8000) ? true : false;  // ���� Ű ���� ����
    }
}

// ���� Ű�� ��� ������ �ִ°�?
bool CKeyMgr::Key_Pressing(int _Key)
{
    return m_bKeyState[_Key];
}

// Ű�� ���ȴٰ� ������ ���� ����
bool CKeyMgr::Key_Up(int _Key)
{
    return m_bPrevKeyState[_Key] && !m_bKeyState[_Key];
}

// Ű�� ó�� ���� ���� ����
bool CKeyMgr::Key_Down(int _Key)
{
    return !m_bPrevKeyState[_Key] && m_bKeyState[_Key];
}

// Ű �Ŵ��� ����
CKeyMgr* CKeyMgr::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    return new CKeyMgr();
}

// ���ҽ� ����
void CKeyMgr::Free()
{
}
