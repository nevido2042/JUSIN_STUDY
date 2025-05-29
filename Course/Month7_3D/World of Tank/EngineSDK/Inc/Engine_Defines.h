#pragma once

#pragma warning (disable : 4251)

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <locale>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <wincodec.h>

#include <d3d11.h>
#include <dxgidebug.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>

#include "DirectXTK/DDSTextureLoader.h"
#include "DirectXTK/WICTextureLoader.h"
#include "DirectXTK/SpriteBatch.h"
#include "DirectXTK/SpriteFont.h"

#include "Fx11/d3dx11effect.h"
#include <d3dcompiler.h>

using namespace DirectX;

#define DIRECTINPUT_VERSION 0x0800

#define AI_TEXTURE_TYPE_MAX aiTextureType_GLTF_METALLIC_ROUGHNESS

#include <dinput.h>

#include <string>
#include <map>
#include <unordered_map>
#include <list>
#include <vector>
#include <memory>
#include <iostream>
#include <functional>
#include <algorithm>

using namespace std;


#include "Engine_Typedef.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Enum.h"
#include "Engine_Function.h"


namespace Engine
{
	const _wstring	g_strTransformTag = TEXT("Com_Transform");

	constexpr _uint g_iBase_WinSizeX = 1920;
	constexpr _uint g_iBase_WinSizeY = 1080;
}

using namespace Engine;

ENGINE_DLL extern _bool g_bWindowResizeRequired;
ENGINE_DLL extern _uint g_iWinSizeX;
ENGINE_DLL extern _uint g_iWinSizeY;
ENGINE_DLL extern HWND g_hWnd;


#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif
