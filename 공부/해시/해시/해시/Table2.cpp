#include "pch.h"
#include "Table2.h"

void CTable2::Initialize(HashFunc* _HashFunc)
{
    m_HashFunc = _HashFunc;
}

void CTable2::Insert(Key _Key, Value _Value)
{
    int iHashValue = m_HashFunc(_Key);

    CSlot2 NewSlot = { _Key, _Value };

    if (Search(_Key) != nullptr)
    {
        cout << "키 중복 오류 발생" << endl;
        return;
    }
    else
    {
        m_TBL[iHashValue].emplace_front(NewSlot);
    }
}

Value CTable2::Delete(Key _Key)
{
    int iHashValue = m_HashFunc(_Key);

    auto Iter = m_TBL[iHashValue].begin();

    while (Iter != m_TBL[iHashValue].end())
    {
        if (Iter->m_Key == _Key)
        {
            CSlot2 DeleteSlot = *Iter;
            m_TBL[iHashValue].erase(Iter);

            return DeleteSlot.m_Value;
        }
        ++Iter;
    }

    return nullptr;
}

Value CTable2::Search(Key _Key)
{
    int iHashValue = m_HashFunc(_Key);

    auto Iter = m_TBL[iHashValue].begin();

    while (Iter != m_TBL[iHashValue].end())
    {
        if (Iter->m_Key == _Key)
        {
            return Iter->m_Value;
        }
        ++Iter;
    }

    return nullptr;
}
