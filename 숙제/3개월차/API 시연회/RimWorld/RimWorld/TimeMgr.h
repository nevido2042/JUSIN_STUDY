#pragma once
class CTimeMgr
{
private:
	CTimeMgr();
	~CTimeMgr();

public:
	int Get_CurrentFrame() { return m_iCurFrame; }

public:
	void Initialize();
	void Update();

public:
	static CTimeMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CTimeMgr;

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
	static CTimeMgr* m_pInstance;

private:
	int m_iCurFrame;
};

