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
	D3DXVECTOR3		vNormal;	// ���� ����(���� ����)

	D3DXVECTOR3		vPos;

	D3DXMATRIX		matWorld;

}INFO;

// ��� : �������� ���� ��(����)�� ��(����)�� ���·� ǥ���� ���� ���� �Ǵ� ����
// 4 x 2 : 4�� 2��
// ��� ���� : ��ü�� ���� ����(ũ��, ȸ��, ��ġ)�� ǥ���ϱ� ����
// 
// ���� ��� : ��� ���� ���� ��ġ�ϴ� ������ ���, ������ ������ ���� ����
// �׵� ��� : ���� ����� ������ �����ϰ� �߻��Ǿ����� ���, �ʱ�ȭ ������ ��� �Ǵ� ũ�Ⱑ 1�� ����̶�� ���Ѵ�.
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
// - ũ��(����) : _11(x), _22(y), _33(z)
// 
// - ȸ��(x��) : _22(cos), _23(sin),  _32(-sin), _33(cos)
// - ȸ��(y��) : _11(cos), _13(-sin), _31(sin),  _33(cos)
// - ȸ��(z��) : _11(cos), _12(sin),  _21(-sin), _22(cos)
// 
// - ��ġ(��ǥ) : _41(x), _42(y), _43(z)

///////////////////////////////////////////////////////////
// ����� ���� ��Ģ

// 1. ���� ����� ���� ���� ����� ���� ���� ��ġ�ؾ� ������ ����
// (1 x 3) * (4 x 4) (x)
// (1 x 4) * (4 x 4) (o)

// 2. ���� ����� ��� ���� ����� ���� ���·� ����� ����
// (1 x 4)* (4 x 4) -> (1 x 4)

// 3. A ��İ� B ����� ���� ���� ��, ��ȯ ��Ģ�� �׻� ��������'��' �ʴ´�.

// 4. �׵� ��İ� ���� ��, �׻� ������ ������ ��İ� ������ ����� ���´�.(�׻� ��ȯ ��Ģ�� ����)
// A * I == A
// A * I == I * A

// 5. A��İ� A����� ������� ������ ���, ����� �׻� �׵� ��ķ� ���´�.
// A * (A^-1) == I -> A / A == I


// 185Cm, �����
// 4m 30CM, �ΰ�, ���� ���� ��
// 2mm, ����

// ���� ��� ���� ����(�׻� �ټ����� ����� �������� �Ǵ� ���� �ƴ�, ���� ������ �߿�)
// �� : ���� = ũ�� * �̵�
// ���� ��� = ũ(��) * ��(��) * ��(��) * ��(��) * ��(��)

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


// ��� ���� �Լ�

// �׵� ����� ������ִ� �Լ�
// D3DXMatrixIdentity(�׵� ����� ����� ���� ����� �ּ�)
// D3DXMatrixIdentity(&m_tInfo.matWorld);

// ũ�� ��ȯ ����� ����� �Լ�
// D3DXMatrixScaling(������� ������ ����� �ּ�, X����, Y����, Z����)
// D3DXMATRIX		matScale;
// D3DXMatrixScaling(&matScale, 1.f, 2.f, 1.f);

// ȸ�� ��ȯ ����� ����� �Լ�
// D3DXMatrixRotationZ(������� ������ ����� �ּ�, ȸ�� ����(����))
// D3DXMATRIX		matRotZ, matRevZ;
// D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(angle));

// ��ġ ��ȯ ����� ����� �Լ�
// D3DXMatrixTranslation(������� ������ ����� �ּ�, x��ġ, y��ġ, z��ġ)
// D3DXMATRIX		matTrans;
// D3DXMatrixTranslation(&matTrans, 400.f, 300.f, 0.f);

// dx���� �����ϴ� ��İ��� �Լ����� ���� ù��° �۾����� ��� ���� ������ ����� �׵� ��ķ� �����.

// D3DXMATRIX		matScale, matRotZ, matTrans;
// 
// D3DXMatrixScaling(&matScale, 1.f, 2.f, 1.f);
// D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(angle));
// D3DXMatrixTranslation(&matTrans, 400.f, 300.f, 0.f);
// 
// m_tInfo.matWorld = matScale * matRotZ * matTrans;

// ���Ϳ� ����� ������ ��������ִ� �Լ�
// D3DXVec3TransformCoord(��� ���� ������ ������ �ּ�(��ġ), ������ ������ ��ġ ����, ���);
// D3DXVec3TransformNormal(��� ���� ������ ������ �ּ�(����), ������ ������ ���� ����, ���);
