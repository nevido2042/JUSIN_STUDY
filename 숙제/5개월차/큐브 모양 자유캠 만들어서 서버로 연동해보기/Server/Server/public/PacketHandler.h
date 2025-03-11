#pragma once
#include "Packet.h"

enum SC_PacketType
{
    PACKET_SC_CREATE_MY_CHARACTER = 0,
    PACKET_SC_CREATE_OTHER_CHARACTER = 1,
    PACKET_SC_DELETE_CHARACTER = 2
};

enum CS_PacketType
{
    PACKET_CS_DELETE_MY_CHARACTER = 100,
    PACKET_CS_KEYUP,
};

class CPacketHandler
{
public:
    static 	void CS_KeyUp(CPacket* pPacket);

public:
    static 	void mp_SC_CreateMyCharacter(CPacket* pPacket, int iID, int iX, int iY);
    static 	void net_CreateMyCharacter(CPacket* pPacket, int& iID, int& iX, int& iY);

    static 	void mp_SC_CreateOtherCharacter(CPacket* pPacket, int iID, int iX, int iY);
    static 	void net_CreateOtherCharacter(CPacket* pPacket, int& iID, int& iX, int& iY);

    static void mp_SC_DeleteCharacter(CPacket* pPacket, int iID);
    static void net_DeleteCharacter(CPacket* pPacket, int& iID);
    static void mp_CS_DeleteMyCharacter(CPacket* pPacket);
};

