#pragma once

#include "Engine_Defines.h"

//����, Ŭ��, ���� ��� ���Ǵ� �������

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
