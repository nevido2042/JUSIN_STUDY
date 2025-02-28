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
    PACKET_CS_DELETE_MY_CHARACTER = 100
};

class CPacketHandler
{
public:
    static 	void SC_CreateMyCharacter(CPacket* pPacket, int iID, int iX, int iY);
    static 	void Recive_CreateMyCharacter(CPacket* pPacket, int& iID, int& iX, int& iY);

    static 	void SC_CreateOtherCharacter(CPacket* pPacket, int iID, int iX, int iY);
    static 	void Recive_CreateOtherCharacter(CPacket* pPacket, int& iID, int& iX, int& iY);

    static void SC_DeleteCharacter(CPacket* pPacket, int iID);
    static void Recive_DeleteCharacter(CPacket* pPacket, int& iID);
    static void CS_DeleteMyCharacter(CPacket* pPacket);
};

