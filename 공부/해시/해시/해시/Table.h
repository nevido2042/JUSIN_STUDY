#pragma once

#include "Slot.h"

#define MAX_TBL 100

typedef int HashFunc(Key k);

class CTable
{
public:
	void Initialize(HashFunc* _HashFunc);
	void Insert(Key _Key, Value _Value);
	Value Delete(Key _Key);
	Value Search(Key _Key);
public:
	CSlot m_TBL[MAX_TBL];
	HashFunc* m_HashFunc;
};

