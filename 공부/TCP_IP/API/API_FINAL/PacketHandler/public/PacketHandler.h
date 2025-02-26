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
    PACKET_CS_DELETE_CHARACTER = 100
};

class CPacketHandler
{
public:
#define SC_CREATE_MY_CHARACTER_SIZE 12
    static 	void mp_SC_CreateMyCharacter(CPacket* clpPacket, int iID, int iX, int iY);
    static 	void net_CreateMyCharacter(CPacket* clpPacket, int& iID, int& iX, int& iY);
#define SC_CREATE_OTHER_CHARACTER_SIZE 12
    static 	void mp_SC_CreateOtherCharacter(CPacket* clpPacket, int iID, int iX, int iY);
    static 	void net_CreateOtherCharacter(CPacket* clpPacket, int& iID, int& iX, int& iY);
};

