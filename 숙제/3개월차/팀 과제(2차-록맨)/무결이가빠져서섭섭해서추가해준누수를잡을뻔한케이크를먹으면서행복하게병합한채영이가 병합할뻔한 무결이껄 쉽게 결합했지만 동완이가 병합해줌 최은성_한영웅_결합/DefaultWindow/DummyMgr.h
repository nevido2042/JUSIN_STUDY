#pragma once
class CDummyMgr
{
private:
	CDummyMgr();
	CDummyMgr(const CDummyMgr&) = delete; //���� ������ ����
	CDummyMgr& operator=(const CDummyMgr&) = delete;//���� ������ ����
	~CDummyMgr();
public:
	static CDummyMgr* Get_Instacne()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CDummyMgr;
		}

		return m_pInstance;
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
private:
	void Release();
private:
	static CDummyMgr* m_pInstance;
};

