#pragma once

#include "Engine_Defines.h"
#include "NetCore_Define.h"

enum class TANK { FURY, TIGER, END };

enum class PacketType
{
    SC_GIVE_ID,

    CS_PING,
    SC_PING,

    CS_JOIN_MATCH,
    SC_START_GAME,
    CS_LOAD_COMPLETE,

    CS_CREATE_MY_CHARACTER,
    SC_CREATE_MY_CHARACTER,

    CS_CREATE_OTHER_CHARACTER,
    SC_CREATE_OTHER_CHARACTER,

    CS_PRESS_W,
    SC_PRESS_W,

    CS_PRESS_S,
    SC_PRESS_S,

    CS_PRESS_A,
    SC_PRESS_A,

    CS_PRESS_D,
    SC_PRESS_D,

    CS_LEFT,
    SC_LEFT,

    CS_RIGHT,
    SC_RIGHT,

    CS_UP,
    SC_UP,

    CS_DOWN,
    SC_DOWN,


    CS_MATRIX_BODY,
    SC_MATRIX_BODY,

    CS_MATRIX_TURRET,
    SC_MATRIX_TURRET,

    CS_MATRIX_GUN,
    SC_MATRIX_GUN,

    CS_TANK_MATRIX,
    SC_TANK_MATRIX,

    CS_POSITION,
    SC_POSITION,

    CS_DELETE_CHARACTER,
    SC_DELETE_CHARACTER,
};

#pragma pack(push, 1)
typedef struct tagPacket_Desc
{
    _int iID{ -1 };
}PACKET_DESC;

typedef struct tagBool_Desc : PACKET_DESC
{
    _bool bBool{ false };
}BOOL_DESC;


typedef struct tagPostion_Desc : PACKET_DESC
{
    _float3 vPos{};
}POSITION_DESC;

typedef struct tagJoinMatch_Desc : PACKET_DESC
{
    TANK eTank{ TANK::END };
}JOIN_MATCH_DESC;

typedef struct tagCreateOtherTank_Desc : POSITION_DESC
{
    TANK eTank{ TANK::END };
}CREATE_OTHER_TANK_DESC;

typedef struct tagMatrix_Desc : PACKET_DESC
{
    _float4x4 Matrix{};
}MATRIX_DESC;

typedef struct tagTankMatrix_Desc : PACKET_DESC
{
    _float4x4 Matrix_Body{};
    _float4x4 Matrix_Turret{};
    _float4x4 Matrix_Gun{};
}TANK_MATRIX_DESC;
#pragma pack(pop)

