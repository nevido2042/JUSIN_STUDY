#pragma once
#include "PacketType.h"

typedef struct tagSessionInfo
{
	_int			iID = { 0 };
	_bool			isJoinMatch = { false };
	_float3			vPosition = {};
	TANK			eTank = { TANK::END };

	_float4x4		Matrix_Body = {};
	_float4x4		Matrix_Turret = {};
	_float4x4		Matrix_Gun = {};

	//이런 것들을 새로운 신입이 왔을 때 쏴줘야함
	//캐릭터의 상태
	//캐릭터의 이동 방향
}SESSION_INFO;
