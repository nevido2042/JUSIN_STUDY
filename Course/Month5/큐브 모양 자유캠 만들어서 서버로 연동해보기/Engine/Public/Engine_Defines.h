#pragma once

#pragma warning (disable : 4251)

#include <d3d9.h>
#include <d3dx9.h>

#include <string>
#include <map>
#include <list>
#include <vector>
using namespace std;

#include "Engine_Struct.h"
#include "Engine_Enum.h"
#include "Engine_Function.h"
#include "Engine_Typedef.h"
#include "Engine_Macro.h"

using namespace Engine;


#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif

#define	VK_MAX		0xff

#define PACKET_CODE (_byte)0x20

#pragma pack(push, 1)
struct tagPACKET_HEADER
{
    _byte byCode;
    _byte bySize;
    _byte byType;
};
#pragma pack(pop)



