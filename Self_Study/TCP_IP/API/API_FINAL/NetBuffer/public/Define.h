#pragma once

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif

typedef char _byte;
#define PACKET_CODE (_byte)0x20

#pragma pack(push, 1)
struct tagPACKET_HEADER
{
    _byte byCode;
    _byte bySize;
    _byte byType;
};
#pragma pack(pop)

