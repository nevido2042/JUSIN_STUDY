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
