#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>

#include <vector>
#include <locale>
#include <iostream>

#include <string>
#include <d3d9.h>
#include <d3dx9.h>

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif

template<class T>
void Safe_Delete(T& p)
{
	if (p != nullptr)
	{
		delete p;   
		p = nullptr;
	}
}

typedef signed char _BYTE;

#define PACKET_CODE (_BYTE)0x20

#pragma pack(push, 1)
struct tagPACKET_HEADER
{
	_BYTE byCode;
	_BYTE bySize;
	_BYTE byType;
};
#pragma pack(pop)

#pragma region Engine Typedef
typedef unsigned char				_ubyte;
typedef signed char					_byte;
typedef char						_char;

typedef unsigned short				_ushort;
typedef signed short				_short;

typedef unsigned int				_uint;
typedef signed int					_int;

typedef unsigned long				_ulong;
typedef signed long					_long;

typedef float						_float;
typedef double						_double;


//typedef wstring						_wstring;
typedef wchar_t						_tchar;
typedef bool						_bool;

typedef D3DXVECTOR2					_float2;
typedef D3DXVECTOR3					_float3;
typedef D3DXVECTOR4					_float4;

typedef D3DXMATRIX					_float4x4;
#pragma endregion
