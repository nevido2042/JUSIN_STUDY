#pragma once
class CCamera
{
private:
	CCamera();
	~CCamera();
public:
	POINT WorldToScreen(float worldX, float worldY)
	{
		POINT screenPoint;
		screenPoint.x = int((worldX - m_fX) * m_fZoom + m_fWidth / 2);
		screenPoint.y = int((worldY - m_fY) * m_fZoom + m_fHeight / 2);
		return screenPoint;
	}

	POINT ScreenToWorld(int screenX, int screenY)
	{
		POINT worldPoint;
		worldPoint.x = int((screenX - m_fWidth / 2) / m_fZoom + m_fX);
		worldPoint.y = int((screenY - m_fHeight / 2) / m_fZoom + m_fY);
		return worldPoint;
	}
	void Move_To_Lerp(float _fX, float _fY);
public:
	void Initialize();
	void Update();
	void Late_Update();
	void Render(HDC hDC);
public:
	float	Get_X() { return m_fX; }
	float	Get_Y() { return m_fY; }
	void	Set_Pos(float _fX, float _fY) { m_fX = _fX, m_fY = _fY; }
	float	Get_Zoom() { return m_fZoom; }
	bool	IsInCameraView(float _fX, float _fY, float _fWidth, float _fHeight);
public:
	static CCamera* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CCamera;

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
	static CCamera* m_pInstance;

	float	m_fX;
	float	m_fY;

	float	m_fWidth;
	float	m_fHeight;
	float		m_fZoom; // �� ���� (1.0 = �⺻ ũ��)

	float	m_fViewWidth;
	float	m_fViewHeight;

	//�����̵�
	float	m_fMoveX;
	float	m_fMoveY;
	bool	m_bMoveLerp;
};
