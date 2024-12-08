#pragma once

#include "Define.h"

class CScrollMgr
{
private:
	CScrollMgr();
	~CScrollMgr();


public:
	float			Get_ScrollX() { return m_fScrollX; }
	float			Get_ScrollY() { return m_fScrollY; }

	void			Set_ScrollX(float _fX) { m_fScrollX += _fX; }
	void			Set_ScrollY(float _fY) { m_fScrollY += _fY; }
	void			SET_DIR(const DIRECTION& _Dir);//�������
	// �÷��̾ �� �ۿ� ������ �÷��̾� ��ġ �θ��±��
	const DIRECTION& Get_Dir() { return m_eDir; } // ��ũ�ѹ���

	bool			Update();

public:
	static CScrollMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CScrollMgr;

		return m_pInstance;
	}

	static void	Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static CScrollMgr* m_pInstance;

	float			    m_fScrollX;
	float			    m_fScrollY;
	////////////////////////////////�������� �ڵ�
	DIRECTION			m_eDir; // ��ũ�ѹ���
	float				m_fCheck; // ��ũ���� �����϶�
	// ��ũ���� �� ���������� üũ�ϴ� ����
	// ��ũ���� �����̴µ��� ��� ������Ʈ�� ����
	float				m_fScrollSpeed; // ��ũ�� �ӵ�
};


