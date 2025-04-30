#include "pch.h"
#include "Define.h"

void Set_Color(unsigned short text)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text);
}
