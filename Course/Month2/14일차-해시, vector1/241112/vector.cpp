#include "pch.h"

int main()
{
	// vector<int>			vecInt;
	// 
	// for (int i = 0; i < 20; ++i)
	// {
	// 	cout << "������ : " << vecInt.size() << "\tī�Ľ�Ƽ : " << vecInt.capacity() << endl;
	// 	vecInt.push_back((i + 1) * 10);
	// }
	// 
	// cout << "-------------------------------------" << endl;
	// vecInt.clear();		// �����̳� �ȿ� �ִ� ��� ���Ҹ� ����
	// 
	// if (vecInt.empty())	// ������ ������ �Ǵ��ϴ� �Լ�
	// {
	// 	cout << "��� ���� �Ϸ�" << endl;
	// 	cout << "������ : " << vecInt.size() << "\tī�Ľ�Ƽ : " << vecInt.capacity() << endl;
	// }

	// size : �����̳� �ȿ� ����� ������ ������ ��ȯ

	// for (size_t i = 0; i < vecInt.size(); ++i)
	// 	cout << vecInt[i] << endl;
	// 
	// vecInt.pop_back();
	// vecInt.pop_back();
	// 
	// cout << "=======================================" << endl;
	// 	
	// 
	// for (size_t i = 0; i < vecInt.size(); ++i)
	// 	cout << vecInt[i] << endl;

	vector<int>		vecTemp;

	vecTemp.push_back(10);
	vecTemp.push_back(20);
	vecTemp.push_back(30);
	vecTemp.push_back(40);

	// front : ���� �� �� ���� ����
	//cout << vecTemp.front() << endl;

	// back : ���� �� �� ���� ����
	//cout << vecTemp.back() << endl;

	vecTemp.front() = 100;
	vecTemp.back()  = 400;

	for (size_t i = 0; i < vecTemp.size(); ++i)
		 	cout << vecTemp[i] << endl;


	//vecTemp.clear();
	//
	//cout << "vecTemp ������ : " << vecTemp.size() << "\t vecTemp ī�Ľ�Ƽ : " << vecTemp.capacity() << endl;
	//
	//// vector<int>().swap(vecTemp);
	//// vecTemp.shrink_to_fit();
	//
	//cout << "vecTemp ������ : " << vecTemp.size() << "\t vecTemp ī�Ľ�Ƽ : " << vecTemp.capacity() << endl;


	// vector<int>		vecSrc;
	// 
	// vecSrc.push_back(100);
	// vecSrc.push_back(200);
	// vecSrc.push_back(300);
	// vecSrc.push_back(400);
	// vecSrc.push_back(500);
	// vecSrc.push_back(600);
	// 
	// cout << "vecTemp ������ : " << vecTemp.size() << "\t vecTempī�Ľ�Ƽ : " << vecTemp.capacity() << endl;
	// cout << "vecSrc ������ : " << vecSrc.size() << "\t vecSrcī�Ľ�Ƽ : " << vecSrc.capacity() << endl;
	// 
	// vecTemp.swap(vecSrc);
	// 
	// cout << "======================after=============================" << endl;
	// 
	// cout << "vecTemp ������ : " << vecTemp.size() << "\t vecTempī�Ľ�Ƽ : " << vecTemp.capacity() << endl;
	// cout << "vecSrc ������ : " << vecSrc.size() << "\t vecSrcī�Ľ�Ƽ : " << vecSrc.capacity() << endl;

	return 0;
}