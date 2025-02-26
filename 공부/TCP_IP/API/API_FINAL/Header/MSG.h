#pragma once

#define PACKET_CODE (char)0x20
       
#pragma pack(push, 1)  // ��ŷ�� 1�� ����

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
    BYTE	byCode;			// ��Ŷ�ڵ� 0x20 ����.
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
#pragma pack(pop)   // ��ŷ�� ���� ���·� �ǵ�����