#include "pch.h"
#include "Define.h"

void GotoXY(Pos _Pos)
{
	COORD Pos;
	Pos.X = _Pos.iX;
	Pos.Y = _Pos.iY;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}
