#pragma once

#define PACKET_CODE (char)0x20
       
#pragma pack(push, 1)  // 패킹을 1로 설정

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
// 패킷헤더.
//
//---------------------------------------------------------------
/*
    BYTE	byCode;			// 패킷코드 0x20 고정.
    BYTE	bySize;			// 패킷 사이즈.
    BYTE	byType;			// 패킷타입.
*/
struct tagPACKET_HEADER
{
    char BYTEbyCode;
    char BYTEbySize;
    char BYTEbyType;
};
//---------------------------------------------------------------
// 클라이언트 자신의 캐릭터 할당		Server -> Client
//
// 서버에 접속시 최초로 받게되는 패킷으로 자신이 할당받은 ID 와
// 자신의 최초 위치, HP 를 받게 된다. (처음에 한번 받게 됨)
// 
// 이 패킷을 받으면 자신의 ID,X,Y,HP 를 저장하고 캐릭터를 생성시켜야 한다.
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
// 다른 클라이언트의 캐릭터 생성 패킷		Server -> Client
//
// 처음 서버에 접속시 이미 접속되어 있던 캐릭터들의 정보
// 또는 게임중에 접속된 클라이언트들의 생성용 정보.
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
// 캐릭터 삭제 패킷						Server -> Client
//
// 캐릭터의 접속해제 또는 캐릭터가 죽었을때 전송됨.
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
#pragma pack(pop)   // 패킹을 이전 상태로 되돌리기