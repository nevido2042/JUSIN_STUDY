#pragma once

#pragma warning (disable : 4251)

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <locale>
#include <fstream>
#include <sstream>

#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>

#include "DirectXTK/DDSTextureLoader.h"
#include "DirectXTK/WICTextureLoader.h"

#include "Fx11/d3dx11effect.h"
#include <d3dcompiler.h>

using namespace DirectX;

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

#include <string>
#include <map>
#include <unordered_map>
#include <list>
#include <vector>
#include <memory>
#include <iostream>
#include <functional>

using namespace std;


#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Enum.h"
#include "Engine_Function.h"
#include "Engine_Typedef.h"

namespace Engine
{
	const _wstring	g_strTransformTag = TEXT("Com_Transform");
}

using namespace Engine;

//���ӱ��̿� ����
//#ifdef _DEBUG
//
//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
//
//#ifndef DBG_NEW 
//
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
//#define new DBG_NEW 
//
//#endif
//#endif



