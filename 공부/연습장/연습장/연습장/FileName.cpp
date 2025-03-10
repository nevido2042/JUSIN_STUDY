#include <iostream>
#include <algorithm>
using namespace std;

typedef struct tagNode {
    int iData = 0;
    int iCost = 0;         // �θ���� ���� ��� (�ڽĿ� �����)
    tagNode* pLeft = nullptr;
    tagNode* pRight = nullptr;
} NODE;

int iMaxLength = 0;  // ���� ����: ��ü �ִ� ���(����)�� ����

// �ش� ��忡�� �θ� �������� ���� �� �ִ� �ִ� ���� ��� ���̸� ��ȯ�ϴ� �Լ�
int Check_Arm_Length(NODE* pRoot) {
    if (!pRoot)
        return 0;

    // ��������� ����, ������ �ִ� ��� ���̸� ����.
    int left_arm = (pRoot->pLeft) ? Check_Arm_Length(pRoot->pLeft) + pRoot->pLeft->iCost : 0;
    int right_arm = (pRoot->pRight) ? Check_Arm_Length(pRoot->pRight) + pRoot->pRight->iCost : 0;

    // ���� ��带 �������� �� �ĺ� ���� (���� ������ ��)
    int candidate = left_arm + right_arm;
    if (candidate > iMaxLength)
        iMaxLength = candidate;

    //// ����� ���: ���� ����� ���
    //if (!pRoot->pLeft && !pRoot->pRight)
    //    cout << "Leaf Node (cost from parent): " << pRoot->iCost << endl;

    // �θ𿡰� ������ ���� ���ʰ� ������ �� �� �� ���
    return max(left_arm, right_arm);
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int iN;
    cin >> iN;

    NODE* pNodes = new NODE[iN];

    // �Է�: iN-1���� ���� ���� (�θ�, �ڽ�, ���)
    for (int i = 0; i < iN - 1; ++i) {
        int iParent, iChild, iCost;
        cin >> iParent >> iChild >> iCost;

        NODE* pParent = &pNodes[iParent - 1];
        NODE* pChild = &pNodes[iChild - 1];
        pChild->iCost = iCost;

        // �����ϰ� ���ʺ��� �Ҵ� (�� �ڽĸ� �ִٰ� ����)
        if (pParent->pLeft == nullptr)
            pParent->pLeft = pChild;
        else
            pParent->pRight = pChild;
    }

    // Ʈ���� ��Ʈ�� 0�� �ε���(A ���)�� �����ϰ� DFS ����
    Check_Arm_Length(&pNodes[0]);

    cout << "�ִ� ���� ����(����): " << iMaxLength << "\n";

    delete[] pNodes;
    return 0;
}
