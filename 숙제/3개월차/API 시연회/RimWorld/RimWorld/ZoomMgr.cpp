#include "pch.h"
#include "ZoomMgr.h"

CZoomMgr* CZoomMgr::m_pInstance = nullptr;

CZoomMgr::CZoomMgr() : m_fZoom(1.f)
{
}

CZoomMgr::~CZoomMgr()
{
}

void CZoomMgr::Set_Zoom(float _fInput)
{
	m_fZoom += _fInput;

		if (m_fZoom < 0.3f)
		{
			m_fZoom = 0.3f;
		}
		else if (m_fZoom > 1.f) 
		{
			m_fZoom = 1.f;
		}
}
