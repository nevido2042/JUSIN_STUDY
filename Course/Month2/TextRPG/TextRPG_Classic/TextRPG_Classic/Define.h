#pragma once

#define TWO_LINE "================================================================="

template <typename T>
void Safe_Delete(T& _p)
{
	if (_p)
	{
		delete _p;
		_p = nullptr;
	}
}