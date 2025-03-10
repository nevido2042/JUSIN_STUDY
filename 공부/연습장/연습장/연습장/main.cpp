
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <list>
#include <map>
#include <stack>
#include <queue>
using namespace std;

typedef struct tagNode
{
    int iData = 0;
    int iCost = 0;
    tagNode* pLeft = nullptr;
    tagNode* pRight = nullptr;
}NODE;

int Check_Arm_Length(NODE* pRoot, NODE* pNodes);

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);

    int iN;
    cin >> iN;

    NODE* pNodes = new NODE[iN];

    for (int i = 0; i < iN - 1; ++i)
    {
        int iParent, iChild, iCost;
        cin >> iParent >> iChild >> iCost;

        NODE* pParent = &pNodes[iParent - 1];
        NODE* pChild = &pNodes[iChild - 1];
        pChild->iCost = iCost;

        if (pParent->pLeft == nullptr)
        {
            pParent->pLeft = pChild;
        }
        else
        {
            pParent->pRight = pChild;
        }
    }

    int iMaxLength = 0;
    for (int i = 0; i < iN; ++i)
    {
        int iLength{ 0 };
        iLength += Check_Arm_Length(pNodes[i].pLeft, pNodes);//왼팔
        //iLength += Check_Arm_Length(pNodes[i].pRight, pNodes);//오른팔
        if (iMaxLength < iLength)
        {
            iMaxLength = iLength;
        }
    }

    cout << iMaxLength;

    delete[] pNodes;
}

int Check_Arm_Length(NODE* pRoot, NODE* pNodes)
{
    if (!pRoot)
        return 0;

    // 좌우 자식이 없으면 리프 노드
    if (!pRoot->pLeft && !pRoot->pRight)
        cout << "Leaf Node: " << pRoot->iCost << endl;

    Check_Arm_Length(pRoot->pLeft, pNodes);
    Check_Arm_Length(pRoot->pRight, pNodes);

    //iTotal += pRoot->iLCost;

    //pRoot = pRoot->pLeft;
    //if (pRoot->iLCost > pRoot->iRCost)
    //{
    //    iTotal += pRoot->iLCost;
    //    pRoot = pRoot->pLeft;
    //}
    //else
    //{
    //    iTotal += pRoot->iRCost;
    //    pRoot = pRoot->pRight;
    //}
}
