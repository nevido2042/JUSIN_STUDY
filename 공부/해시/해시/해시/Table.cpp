#include "pch.h"
#include "Table.h"

void CTable::Initialize(HashFunc* _HashFunc)
{
	for (int i = 0; i < MAX_TBL; ++i)
	{
		m_TBL[i].m_Status = EMPTY;
	}

	m_HashFunc = _HashFunc;
}

void CTable::Insert(Key _Key, Value _Value)
{
	int HashValue = m_HashFunc(_Key);
	m_TBL[HashValue].m_Value = _Value;
	m_TBL[HashValue].m_Key = _Key;
	m_TBL[HashValue].m_Status = INUSE;
}

Value CTable::Delete(Key _Key)
{
	int HashValue = m_HashFunc(_Key);
	if (m_TBL[HashValue].m_Status != INUSE)
	{
		return nullptr;
	}
	else
	{
		m_TBL[HashValue].m_Status = DELETED;
		return m_TBL[HashValue].m_Value;
	}
}

Value CTable::Search(Key _Key)
{
	int HashValue = m_HashFunc(_Key);

	if (m_TBL[HashValue].m_Status != INUSE)
	{
		return nullptr;
	}
	else
	{
		return m_TBL[HashValue].m_Value;
	}
}
