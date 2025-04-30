#pragma once
#include "Slot2.h"
//#include <forward_list>
#include <list>

#define MAX_TBL 100

typedef int HashFunc(Key k);

class CTable2
{
public:
	void Initialize(HashFunc* _HashFunc);
	void Insert(Key _Key, Value _Value);
	Value Delete(Key _Key);
	Value Search(Key _Key);
public:
	list<CSlot2> m_TBL[MAX_TBL];
	HashFunc* m_HashFunc;
};

