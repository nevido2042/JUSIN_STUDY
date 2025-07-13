#pragma once

#include "Engine_Defines.h"

//using namespace Engine;

namespace Client
{
	typedef struct tagGameResult
	{
		_bool bIsVictory = { false };
		_float fTotalDamage = { 0.f };
		_uint iNumDamageModule = { 0 };
	}GAME_RESULT;

}
