#pragma once

#include "Engine_Defines.h"

//엔진, 클라, 서버 모두 사용되는 헤더파일

#define PACKET_CODE (_byte)0x20
#define SESSION_MAX 10

#pragma pack(push, 1)
typedef struct tagPacket_Header
{
    _byte byCode;
    _byte bySize;
    _byte byType;
}PACKET_HEADER;
#pragma pack(pop)
