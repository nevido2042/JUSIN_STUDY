#pragma once
#include "Person.h"

typedef int Key;		//주민등록번호
typedef CPerson* Value;

class CSlot2
{
public:
	Key m_Key;
	Value m_Value;
};

