#pragma once
class CZoomMgr
{
private:
	CZoomMgr();
	~CZoomMgr();

public:
	float			Get_Zoom() { return m_fZoom; }

	void			Set_Zoom(float _fInput);

public:
	static CZoomMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CZoomMgr;

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
	static CZoomMgr* m_pInstance;

	float			    m_fZoom;
};

