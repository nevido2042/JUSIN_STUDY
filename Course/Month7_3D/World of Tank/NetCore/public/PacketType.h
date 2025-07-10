#pragma once

#include "Engine_Defines.h"
#include "NetCore_Define.h"

enum class CUSTOM3D     { MONKEY, DRAGONHEAD, DRUM, SPEAKER, GUITAR, SHARK, GOLDPOT, END };
enum class TANK         { FURY, TIGER, END };
enum class MODULE       { ENGINE, CHASSIS, TURRET, GUN, SURVEYING_DEVICE, RADIO, FUEL_TANK, AMMO_BAY, END };
enum class MODULE_STATE { DESTROYED, DAMAGED, FUNCTIONAL, END };
enum class TEAM         { A, B, END };

enum class PacketType
{
    SC_GIVE_ID,

    CS_PING,
    SC_PING,

    CS_JOIN_MATCH,
    SC_START_GAME,
    CS_LOAD_COMPLETE,
    CS_END_GAME,

	CS_RETURN_HANGER,
    SC_RETURN_HANGER,

    CS_CREATE_MY_CHARACTER,
    SC_CREATE_MY_CHARACTER,

    CS_CREATE_OTHER_CHARACTER,
    SC_CREATE_OTHER_CHARACTER,

    SC_CREATE_MINIMAP,

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

    CS_FIRE,
    SC_FIRE,

    CS_DIG,
    SC_DIG,

    CS_MODULE_STATE,
    SC_MODULE_STATE,

    CS_TAKE_DAMAGE,
    SC_TAKE_DAMAGE,

    CS_HIT_MODULE,
    SC_HIT_MODULE,

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

typedef struct tagHitModule_Desc : PACKET_DESC
{
    _int            iTargetID{ -1 };
    MODULE          eModule{ MODULE::END };
    //_float3         vFirePos = {};
}HIT_MODULE_DESC;

typedef struct tagModuleState_Desc : PACKET_DESC
{
    _int            iTargetID{ -1 };
	MODULE          eModule{ MODULE::END };
    MODULE_STATE    eState{ MODULE_STATE::END };
}MODULE_STATE_DESC;

typedef struct tagTakeDamageDesc : PACKET_DESC
{
    _int    iTargetID{ -1 };
    _float  fDamage{ 0 };
    _float3 vFirePos = {};
}TAKEDAMAGE_DESC;

typedef struct tagBool_Desc : PACKET_DESC
{
    _bool bBool{ false };
}BOOL_DESC;


typedef struct tagPostion_Desc : PACKET_DESC
{
    _float3 vPos{};
}POSITION_DESC;

typedef struct tagSpawnTankDesc : POSITION_DESC
{
    TEAM eTeam = { TEAM::END };
}SPAWN_TANK_DESC;

typedef struct tagJoinMatch_Desc : PACKET_DESC
{
    TANK eTank{ TANK::END };
    _float4 vBodyColor      { 1.f, 1.f, 1.f, 1.f };
    _float4 vTurretColor    { 1.f, 1.f, 1.f, 1.f };
    _float4 vGunColor       { 1.f, 1.f, 1.f, 1.f };
    CUSTOM3D e3DCustom = { CUSTOM3D::END };
}JOIN_MATCH_DESC;

typedef struct tagCreateOtherTank_Desc : SPAWN_TANK_DESC
{
    TANK eTank{ TANK::END };
    _float4 vBodyColor      { 1.f, 1.f, 1.f, 1.f };
    _float4 vTurretColor    { 1.f, 1.f, 1.f, 1.f };
    _float4 vGunColor       { 1.f, 1.f, 1.f, 1.f };
    CUSTOM3D e3DCustom = { CUSTOM3D::END };
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

