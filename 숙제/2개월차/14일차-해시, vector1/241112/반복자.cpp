#include "pch.h"
#include <list>

int main()
{
	// �ݺ���(iterator) : �����Ϳ� ����� ����

	vector<int>		vecInt;

	vecInt.push_back(10);
	vecInt.push_back(20);
	vecInt.push_back(30);
	vecInt.push_back(40);

	vector<int>::iterator		iter = vecInt.begin();
	iter += 2;

	// �߰� ���� �Լ�
	// vecInt.insert(iter, 500);

	// �߰� ���� �Լ�

	iter = vecInt.erase(iter);		// erase ���� ���� ����ų ����� ��ȯ
	cout << *iter << endl;

	// for (vector<int>::iterator	iter = vecInt.begin();
	// 	iter != vecInt.end();  ++iter)
	// {
	// 	cout << *iter << endl;
	// }
	

	// 1. ��� �ݺ��� : �����̳� ���� ���� �б⸸ ������ �ݺ���
	// 2. �Է� �ݺ��� : �����̳� ���� ���� �б�, ���� ��� ������ �ݺ���
	// 3. ������(������) �ݺ��� : �����̳� ���� ���Ҹ� ���������θ� �̵��Ͽ� ������ ����
	// 4. ����� �ݺ��� : �����̳� ���� ���Ҹ� ��������� ��� �̵��Ͽ� ������ ����
	// 5. ���� ���� �ݺ��� : �����̳� ���� ������ ���� ������ ���

	return 0;
}

// vector �� �̿��Ͽ� ����ǥ ���α׷� �����
// 1. �߰� 2. ��� 3. �˻� 4. ���� 5. ����