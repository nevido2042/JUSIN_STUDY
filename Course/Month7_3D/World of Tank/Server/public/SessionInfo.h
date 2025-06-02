#pragma once
#include "PacketType.h"

typedef struct tagSessionInfo
{
	//쓰고있음
	_int			iID = { 0 };
	_bool			isJoinMatch = { false };
	TANK			eTank = { TANK::END };
	_float4			vBodyColor = { 1.f, 1.f, 1.f, 1.f };
	_float4			vTurretColor = { 1.f, 1.f, 1.f, 1.f };
	_float4			vGunColor = { 1.f, 1.f, 1.f, 1.f };

	//안쓰고있음
	_float3			vPosition = {};

	//이런 것들을 새로운 신입이 왔을 때 쏴줘야함
	//캐릭터의 상태
	//캐릭터의 이동 방향
}SESSION_INFO;
