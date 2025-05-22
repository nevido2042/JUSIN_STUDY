#pragma once

#include "Engine_Defines.h"
#include "NetCore_Define.h"

enum class PacketType
{
    SC_GIVE_ID,

    CS_PING,
    SC_PING,

    CS_JOIN_MATCH_QUEUE,
    SC_START_GAME,

    CS_POSITION,
    SC_POSITION,

    CS_CREATE_MY_CHARACTER,
    SC_CREATE_MY_CHARACTER,

    CS_CREATE_OTHER_CHARACTER,
    SC_CREATE_OTHER_CHARACTER,

    CS_DELETE_CHARACTER,
    SC_DELETE_CHARACTER,
};

typedef struct tagPacket_Desc
{
    _uint iID{ SESSION_MAX }; //SESSION_MAX Àº ¼­¹ö
}PACKET_DESC;

typedef struct tagPostion_Desc : PACKET_DESC
{
    _float3 vPos{};
}POSITION_DESC;

