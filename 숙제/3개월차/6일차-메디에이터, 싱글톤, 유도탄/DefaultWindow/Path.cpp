#include "pch.h"
#include "Path.h"

CPath::CPath()
{
}

CPath::~CPath()
{
    Release();
}

void CPath::Initialize()
{
}

int CPath::Update()
{
    return 0;
}

void CPath::Late_Update()
{
}

void CPath::Render(HDC hDC)
{
    for (auto Iter = m_PointList.begin(); Iter != m_PointList.end();)
    {
        auto NextIter = Iter;
        ++NextIter;

        if (NextIter == m_PointList.end())
        {
            break;
        }
        else
        {
            MoveToEx(hDC, Iter->x, Iter->y, nullptr);
            LineTo(hDC, NextIter->x, NextIter->y);
            ++Iter;
        }
    }
}

void CPath::Release()
{
}
