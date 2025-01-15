#pragma once

#define WINCX		800
#define WINCY		600

#define	PURE		= 0
#define PI			3.141592f

extern HWND		g_hWnd;


template<typename T>
void Safe_Delete(T& Temp)
{
	if (Temp)
	{
		delete Temp;
		Temp = nullptr;
	}
}

static D3DXVECTOR3		Get_Mouse()
{
	POINT		pt{};

	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	return D3DXVECTOR3((float)pt.x, (float)pt.y, 0.f );
}

typedef struct tagInfo
{
	D3DXVECTOR3		vDir;
	D3DXVECTOR3		vLook;
	D3DXVECTOR3		vNormal;	// 법선 벡터(방향 벡터)

	D3DXVECTOR3		vPos;

	D3DXMATRIX		matWorld;

}INFO;

// 행렬 : 여러가지 수를 행(가로)과 열(세로)의 형태로 표현해 놓은 구조 또는 상태
// 4 x 2 : 4행 2열
// 사용 이유 : 객체의 여러 상태(크기, 회전, 위치)를 표현하기 위함
// 
// 정방 행렬 : 행과 열의 수가 일치하는 상태의 행렬, 연산의 편리성을 위해 제공
// 항등 행렬 : 연산 결과가 언제나 동일하게 발생되어지는 행렬, 초기화 상태의 행렬 또는 크기가 1인 행렬이라고도 말한다.
/*
1	0	0	0
0	1	0	0
0	0	1	0
0	0	0	1
*/

// matWorld.m[0][1]
// matWorld._11

//  _11,  _12,  _13,  _14
//  _21,  _22,  _23,  _24
//  _31,  _32,  _33,  _34
//  _41,  _42,  _43,  _44
// 
// - 크기(배율) : _11(x), _22(y), _33(z)
// 
// - 회전(x축) : _22(cos), _23(sin),  _32(-sin), _33(cos)
// - 회전(y축) : _11(cos), _13(-sin), _31(sin),  _33(cos)
// - 회전(z축) : _11(cos), _12(sin),  _21(-sin), _22(cos)
// 
// - 위치(좌표) : _41(x), _42(y), _43(z)

///////////////////////////////////////////////////////////
// 행렬의 곱셈 법칙

// 1. 앞의 행렬의 열과 뒤의 행렬의 행의 값이 일치해야 곱셈이 가능
// (1 x 3) * (4 x 4) (x)
// (1 x 4) * (4 x 4) (o)

// 2. 앞의 행렬의 행과 뒤의 행렬의 열의 상태로 결과가 도출
// (1 x 4)* (4 x 4) -> (1 x 4)

// 3. A 행렬과 B 행렬의 곱셈 연산 시, 교환 법칙이 항상 성립하지'는' 않는다.

// 4. 항등 행렬과 곱셈 시, 항상 곱셈을 수행한 행렬과 동일한 결과가 나온다.(항상 교환 법칙이 성립)
// A * I == A
// A * I == I * A

// 5. A행렬과 A행렬의 역행렬을 곱셈할 경우, 결과는 항상 항등 행렬로 나온다.
// A * (A^-1) == I -> A / A == I


// 185Cm, 노랑색
// 4m 30CM, 두개, 팔이 여섯 개
// 2mm, 개미

// 월드 행렬 구성 순서(항상 다섯개의 행렬이 갖춰져야 되는 것은 아님, 곱셈 순서가 중요)
// 예 : 월드 = 크기 * 이동
// 월드 행렬 = 크(기) * 자(전) * 이(동) * 공(전) * 부(모)

/*
					 World
(x, y, z)	*		Matrix			 ->	   (x`, y`, z`)
Local Vtx									World Vtx

*/


/*
					_11, _12, _13,
x, y, z		*		_21, _22, _23,
					_31, _32, _33,
*/

/*

1 2    *   5 6		->	1 * 5 + 2 * 7		1 * 6 + 2 * 8	-> 19	22
3 4 	   7 8			3 * 5 + 4 * 7		3 * 6 + 4 * 8	   43   50

5 6	   *   1 2		->  5 * 1 + 6 * 3						-> 23 
7 8		   3 4 	

*/


// 행렬 관련 함수

// 항등 행렬을 만들어주는 함수
// D3DXMatrixIdentity(항등 행렬을 만들기 위한 행렬의 주소)
// D3DXMatrixIdentity(&m_tInfo.matWorld);

// 크기 변환 행렬을 만드는 함수
// D3DXMatrixScaling(결과값을 저장할 행렬의 주소, X배율, Y배율, Z배율)
// D3DXMATRIX		matScale;
// D3DXMatrixScaling(&matScale, 1.f, 2.f, 1.f);

// 회전 변환 행렬을 만드는 함수
// D3DXMatrixRotationZ(결과값을 저장할 행렬의 주소, 회전 각도(라디안))
// D3DXMATRIX		matRotZ, matRevZ;
// D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(angle));

// 위치 변환 행렬을 만드는 함수
// D3DXMatrixTranslation(결과값을 저장할 행렬의 주소, x위치, y위치, z위치)
// D3DXMATRIX		matTrans;
// D3DXMatrixTranslation(&matTrans, 400.f, 300.f, 0.f);

// dx에서 제공하는 행렬관련 함수들은 가장 첫번째 작업으로 결과 값을 저장할 행렬을 항등 행렬로 만든다.

// D3DXMATRIX		matScale, matRotZ, matTrans;
// 
// D3DXMatrixScaling(&matScale, 1.f, 2.f, 1.f);
// D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(angle));
// D3DXMatrixTranslation(&matTrans, 400.f, 300.f, 0.f);
// 
// m_tInfo.matWorld = matScale * matRotZ * matTrans;

// 벡터와 행렬의 곱셈을 수행시켜주는 함수
// D3DXVec3TransformCoord(결과 값을 저장할 벡터의 주소(위치), 곱셈을 수행할 위치 벡터, 행렬);
// D3DXVec3TransformNormal(결과 값을 저장할 벡터의 주소(방향), 곱셈을 수행할 방향 벡터, 행렬);
