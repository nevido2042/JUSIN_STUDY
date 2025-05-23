#pragma once

#include "Engine_Defines.h"
#include "NetCore_Define.h"

enum class PacketType
{
    SC_GIVE_ID,

    CS_PING,
    SC_PING,

    CS_JOIN_MATCH,
    SC_START_GAME,
    CS_LOAD_COMPLETE,

    CS_MATRIX,
    SC_MATRIX,

    CS_TANK_MATRIX,
    SC_TANK_MATRIX,

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

typedef struct tagMatrix_Desc : PACKET_DESC
{
    _float4x4 Matrix{};
}MATRIX_DESC;

typedef struct tagTankMatrix_Desc : PACKET_DESC
{
    _float4x4 Matrix_Body{};
    //_float4x4 Matrix_Turret{};
    //_float4x4 Matrix_Gun{};
}TANK_MATRIX_DESC;

