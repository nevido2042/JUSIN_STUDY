#pragma once

typedef struct tagPosition
{
	int iX;
	int iY;

	tagPosition()
		:iX(0), iY(0)
	{

	}
	tagPosition(int _iX, int _iY)
		:iX(_iX), iY(_iY)
	{

	}
}Pos;

template<typename T>
void Safe_Delete(T& _p)
{
	if (_p)
	{
		delete _p;
		_p = nullptr;
	}
}

void GotoXY(Pos _Pos);