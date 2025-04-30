#include <iostream>
#include <algorithm>
using namespace std;

typedef struct tagNode {
    int iData = 0;
    int iCost = 0;         // 부모와의 연결 비용 (자식에 저장됨)
    tagNode* pLeft = nullptr;
    tagNode* pRight = nullptr;
} NODE;

int iMaxLength = 0;  // 전역 변수: 전체 최대 경로(지름)를 저장

// 해당 노드에서 부모 방향으로 보낼 수 있는 최대 하향 경로 길이를 반환하는 함수
int Check_Arm_Length(NODE* pRoot) {
    if (!pRoot)
        return 0;

    // 재귀적으로 왼쪽, 오른쪽 최대 경로 길이를 구함.
    int left_arm = (pRoot->pLeft) ? Check_Arm_Length(pRoot->pLeft) + pRoot->pLeft->iCost : 0;
    int right_arm = (pRoot->pRight) ? Check_Arm_Length(pRoot->pRight) + pRoot->pRight->iCost : 0;

    // 현재 노드를 기준으로 한 후보 길이 (양팔 길이의 합)
    int candidate = left_arm + right_arm;
    if (candidate > iMaxLength)
        iMaxLength = candidate;

    //// 디버그 출력: 리프 노드일 경우
    //if (!pRoot->pLeft && !pRoot->pRight)
    //    cout << "Leaf Node (cost from parent): " << pRoot->iCost << endl;

    // 부모에게 전달할 값은 왼쪽과 오른쪽 중 더 긴 경로
    return max(left_arm, right_arm);
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int iN;
    cin >> iN;

    NODE* pNodes = new NODE[iN];

    // 입력: iN-1개의 간선 정보 (부모, 자식, 비용)
    for (int i = 0; i < iN - 1; ++i) {
        int iParent, iChild, iCost;
        cin >> iParent >> iChild >> iCost;

        NODE* pParent = &pNodes[iParent - 1];
        NODE* pChild = &pNodes[iChild - 1];
        pChild->iCost = iCost;

        // 간단하게 왼쪽부터 할당 (두 자식만 있다고 가정)
        if (pParent->pLeft == nullptr)
            pParent->pLeft = pChild;
        else
            pParent->pRight = pChild;
    }

    // 트리의 루트를 0번 인덱스(A 노드)로 가정하고 DFS 실행
    Check_Arm_Length(&pNodes[0]);

    cout << "최대 양팔 길이(지름): " << iMaxLength << "\n";

    delete[] pNodes;
    return 0;
}
