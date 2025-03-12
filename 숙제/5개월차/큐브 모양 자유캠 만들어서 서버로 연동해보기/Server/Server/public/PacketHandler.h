#pragma once
#include "Engine_Defines.h"
#include "Packet.h"


enum SC_PacketType
{
    PACKET_SC_CREATE_MY_CHARACTER = 0,
    PACKET_SC_CREATE_OTHER_CHARACTER = 1,
    PACKET_SC_DELETE_CHARACTER = 2,
    PACKET_SC_MOVE_STOP,
};

enum CS_PacketType
{
    PACKET_CS_DELETE_MY_CHARACTER = 100,
    PACKET_CS_MOVE_START,
    PACKET_CS_MOVE_STOP,
};

class CPacketHandler
{
public:
    static 	void mp_CS_Move_Start(CPacket* pPacket, _float3& Pos);
    static 	void net_Move_Start(CPacket* pPacket, _float3& Pos);
public:
    static 	void mp_CS_Move_Stop(CPacket* pPacket, _float3& Pos);
    static 	void net_Move_Stop(CPacket* pPacket, _float3& Pos);

    static 	void mp_SC_Move_Stop(CPacket* pPacket, _float3& Pos, int iID);
    static 	void net_Move_Stop(CPacket* pPacket, _float3& Pos, int& iID);


public:
    static 	void mp_SC_CreateMyCharacter(CPacket* pPacket, int iID, _float3& Pos);
    static 	void net_CreateMyCharacter(CPacket* pPacket, int& iID, _float3& Pos);

    static 	void mp_SC_CreateOtherCharacter(CPacket* pPacket, int iID, _float3& Pos);
    static 	void net_CreateOtherCharacter(CPacket* pPacket, int& iID, _float3& Pos);

    static void mp_SC_DeleteCharacter(CPacket* pPacket, int iID);
    static void net_DeleteCharacter(CPacket* pPacket, int& iID);
    static void mp_CS_DeleteMyCharacter(CPacket* pPacket);
};

