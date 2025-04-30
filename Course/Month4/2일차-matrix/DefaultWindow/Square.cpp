#include "pch.h"
#include "Square.h"

CSquare::CSquare()
	:m_fAngle(0.f), m_fSize(0.f), m_iRotPoint(0)
{
	ZeroMemory(&m_vLocalPoint, sizeof(m_vLocalPoint));
	ZeroMemory(&m_vWorldPoint, sizeof(m_vWorldPoint));

	ZeroMemory(&m_vLocalPoint_Center, sizeof(D3DXVECTOR3));
	ZeroMemory(&m_vWorldPoint_Center, sizeof(D3DXVECTOR3));
}

CSquare::~CSquare()
{
	Release();
}

void CSquare::Initialize()
{
	m_fSize = 50.f;

	m_tInfo.vPos = { m_fSize, WINCY * 0.5f - m_fSize, 0.f };

	m_vLocalPoint[0] = { -m_fSize, -m_fSize, 0.f };
	m_vLocalPoint[1] = { m_fSize, -m_fSize, 0.f };
	m_vLocalPoint[2] = { m_fSize, m_fSize, 0.f };
	m_vLocalPoint[3] = { -m_fSize, m_fSize, 0.f };

	m_vLocalPoint_Center = { 0.f, 0.f, 0.f };

	m_iRotPoint = 2;
}

void CSquare::Update()
{
	//각도를 증가 시킨다.
	m_fAngle += D3DXToRadian(.5f);

	//충돌 체크할 포인트 설정
	int iCheckPoint = m_iRotPoint - 1;
	if (iCheckPoint < 0)
	{
		iCheckPoint = 3;
	}

	//충돌 체크할 포인트가 충돌하면
	if (m_vWorldPoint[iCheckPoint].y > WINCY * 0.5f)
	{
		//현재 회전의 중점으로 하는 포인트에 따라, 사각형의 중점을 이동 시킨다.[하드코딩, 규칙을 못 찾음]
		//처음 시작점(3) 포인트 차이만큼 이동시키는 것 같긴함
		//m_tInfo.vPos += 2.f * (m_vLocalPoint[3] - m_vLocalPoint[m_iRotPoint]);

		switch (m_iRotPoint)
		{
		case 0:
			m_tInfo.vPos += {2.f * m_fSize, -2.0f * m_fSize, 0.f};
			break;
		case 1:
			m_tInfo.vPos += {4.f * m_fSize, 0.f, 0.f};
			break;
		case 2:
			m_tInfo.vPos += {2.f * m_fSize, 2.f * m_fSize, 0.f};
			break;
		case 3:
			m_tInfo.vPos += {0.f, 0.f, 0.f};
			break;
		default:
			break;
		}

		//체크 하던 포인트로 회전 중점을 변경
		m_iRotPoint = iCheckPoint;
	}

	Update_WorldMatrix();
}

void CSquare::Update_WorldMatrix()
{
	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);

	D3DXMATRIX matTransMove;
	D3DXMatrixIdentity(&matTransMove);
	D3DXMatrixTranslation(&matTransMove, -m_vLocalPoint[m_iRotPoint].x, -m_vLocalPoint[m_iRotPoint].y, -m_vLocalPoint[m_iRotPoint].z);

	D3DXMATRIX matRotZ;
	D3DXMatrixRotationZ(&matRotZ, m_fAngle);

	D3DXMATRIX matTransBack;
	D3DXMatrixIdentity(&matTransBack);
	D3DXMatrixTranslation(&matTransBack, m_vLocalPoint[m_iRotPoint].x, m_vLocalPoint[m_iRotPoint].y, m_vLocalPoint[m_iRotPoint].z);

	D3DXMATRIX matTrans;
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, m_tInfo.vPos.z);

	// 행렬 조합: 스케일 * (중심 이동 * 회전 * 원위치 이동) * 최종 이동
	m_tInfo.matWorld = matScale * (matTransMove * matRotZ * matTransBack) * matTrans;

	//m_tInfo.matWorld = matScale * matRotZ * matTrans;

}

void CSquare::Render(HDC hDC)
{
	//사각형
	for (int i = 0; i < 4; ++i)
	{
		//D3DXVECTOR3 vTemp = m_vLocalPoint[i];
		//vTemp -= { m_fSize, m_fSize, 0.f };
		//D3DXVec3TransformCoord(&m_vWorldPoint[i], &vTemp, &m_tInfo.matWorld);

		D3DXVec3TransformCoord(&m_vWorldPoint[i], &m_vLocalPoint[i], &m_tInfo.matWorld);
	}

	MoveToEx(hDC, m_vWorldPoint[0].x, m_vWorldPoint[0].y, nullptr);
	LineTo(hDC, m_vWorldPoint[1].x, m_vWorldPoint[1].y);
	LineTo(hDC, m_vWorldPoint[2].x, m_vWorldPoint[2].y);
	LineTo(hDC, m_vWorldPoint[3].x, m_vWorldPoint[3].y);
	LineTo(hDC, m_vWorldPoint[0].x, m_vWorldPoint[0].y);
	
	//중점
	D3DXVec3TransformCoord(&m_vWorldPoint_Center, &m_vLocalPoint_Center, &m_tInfo.matWorld);
	int iEllipseSize = 20;
	Ellipse(hDC, 
		m_vWorldPoint_Center.x - iEllipseSize,
		m_vWorldPoint_Center.y - iEllipseSize,
		m_vWorldPoint_Center.x + iEllipseSize,
		m_vWorldPoint_Center.y + iEllipseSize);

	TCHAR buffer[64]; //저장할 문자열 버퍼
	// 문자열로 변환하여 출력 준비
	for (int i = 0; i < 4; ++i)
	{
		_stprintf_s(buffer, _T("(%d)[%.0f, %.0f]"), i, m_vWorldPoint[i].x, m_vWorldPoint[i].y);
		// 화면에 출력 (10, 10 위치)
		TextOut(hDC, m_vWorldPoint[i].x, m_vWorldPoint[i].y, buffer, _tcslen(buffer));
	}

	_stprintf_s(buffer, _T("(O)[%.0f, %.0f]"), m_tInfo.vPos.x, m_tInfo.vPos.y);
	// 화면에 출력 (10, 10 위치)
	TextOut(hDC, m_tInfo.vPos.x, m_tInfo.vPos.y, buffer, _tcslen(buffer));
}

void CSquare::Release()
{
}
