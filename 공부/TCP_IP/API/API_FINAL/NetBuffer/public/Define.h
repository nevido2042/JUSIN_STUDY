#pragma once

typedef char _byte;

#define DEFAULT_BUF_SIZE 1024
#define PACKET_CODE (_byte)0x20

#pragma pack(push, 1)
struct tagPACKET_HEADER
{
    _byte byCode;
    _byte bySize;
    _byte byType;
};
#pragma pack(pop)

