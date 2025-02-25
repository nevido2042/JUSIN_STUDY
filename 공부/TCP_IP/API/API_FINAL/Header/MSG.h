#pragma once

#pragma region Packet

#pragma pack(1)

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

//---------------------------------------------------------------
// ��Ŷ���.
//
//---------------------------------------------------------------
/*
    BYTE	byCode;			// ��Ŷ�ڵ� 0x89 ����.
    BYTE	bySize;			// ��Ŷ ������.
    BYTE	byType;			// ��ŶŸ��.
*/
struct tagPACKET_HEADER
{
    char BYTEbyCode;
    char BYTEbySize;
    char BYTEbyType;
};
//---------------------------------------------------------------
// Ŭ���̾�Ʈ �ڽ��� ĳ���� �Ҵ�		Server -> Client
//
// ������ ���ӽ� ���ʷ� �ްԵǴ� ��Ŷ���� �ڽ��� �Ҵ���� ID ��
// �ڽ��� ���� ��ġ, HP �� �ް� �ȴ�. (ó���� �ѹ� �ް� ��)
// 
// �� ��Ŷ�� ������ �ڽ��� ID,X,Y,HP �� �����ϰ� ĳ���͸� �������Ѿ� �Ѵ�.
//
//	4	-	ID
//	1	-	Direction	(LL / RR)
//	2	-	X
//	2	-	Y
//	1	-	HP
//
//---------------------------------------------------------------
struct tagPACKET_SC_CREATE_MY_CHARACTER
{
    int iID;
    int iX;
    int iY;
};
//---------------------------------------------------------------
// �ٸ� Ŭ���̾�Ʈ�� ĳ���� ���� ��Ŷ		Server -> Client
//
// ó�� ������ ���ӽ� �̹� ���ӵǾ� �ִ� ĳ���͵��� ����
// �Ǵ� �����߿� ���ӵ� Ŭ���̾�Ʈ���� ������ ����.
//
//
//	4	-	ID
//	1	-	Direction	(LL / RR)
//	2	-	X
//	2	-	Y
//	1	-	HP
//
//---------------------------------------------------------------
struct tagPACKET_SC_CREATE_OTHER_CHARACTER
{
    int iID;
    int iX;
    int iY;
};
//---------------------------------------------------------------
// ĳ���� ���� ��Ŷ						Server -> Client
//
// ĳ������ �������� �Ǵ� ĳ���Ͱ� �׾����� ���۵�.
//
//	4	-	ID
//
//---------------------------------------------------------------
struct tagPACKET_SC_DELETE_CHARACTER
{
    int iID;
};

struct tagPACKET_CS_DELETE_CHARACTER
{
    int iID;
};
#pragma endregion

#pragma region MSG
#define MSG_SIZE 16

enum MSG_TYPE 
{
    ALLOC_ID, CREATE_PLAYER, DELETE_PLAYER,
    MOVE_RIGHT_PLAYER,
    MOVE_LEFT_PLAYER,
    STOP_RIGHT_PLAYER,
    STOP_LEFT_PLAYER
};

struct CLIENT 
{
    int x = 0;
    int y = 0;
    int id = 0;
};

struct MSG_BASE
{
    int zero[4] = { 0, };
};

struct MSG_ID
{
    int type{ 0 };
    int iID{ 0 };
    int zero[2] = { 0, };
};

struct MSG_ALLOC_ID 
{
    int type = ALLOC_ID;
    int id = 0;
    int zero[2] = { 0, };
};

struct MSG_CREATE_PLAYER
{
    int type = CREATE_PLAYER;
    int id = 0;
    int x = 0;
    int y = 0;
};

struct MSG_DELETE_PLAYER
{
    int type = DELETE_PLAYER;
    int id = 0;
    int zero[2] = { 0, };
};

struct MSG_MOVE_RIGHT_PLAYER
{
    int type = MOVE_RIGHT_PLAYER;
    int id = 0;
    int x = 0;
    int y = 0;
};

struct MSG_MOVE_LEFT_PLAYER
{
    int type = MOVE_LEFT_PLAYER;
    int id = 0;
    int x = 0;
    int y = 0;
};

struct MSG_STOP_LEFT_PLAYER
{
    int type = STOP_LEFT_PLAYER;
    int id = 0;
    int x = 0;
    int y = 0;
};

struct MSG_STOP_RIGHT_PLAYER
{
    int type = STOP_RIGHT_PLAYER;
    int id = 0;
    int x = 0;
    int y = 0;
};
#pragma endregion
