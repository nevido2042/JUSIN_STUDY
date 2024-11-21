#pragma once
#include "Person.h"

typedef int Key;		//주민등록번호
typedef CPerson* Value;

enum SlotStatus { EMPTY, DELETED, INUSE };

class CSlot
{
public:
	Key m_Key;
	Value m_Value;
	enum SlotStatus m_Status;
};

