#include "pch.h"

int main()
{
	// vector<int>			vecInt;
	// 
	// for (int i = 0; i < 20; ++i)
	// {
	// 	cout << "사이즈 : " << vecInt.size() << "\t카파시티 : " << vecInt.capacity() << endl;
	// 	vecInt.push_back((i + 1) * 10);
	// }
	// 
	// cout << "-------------------------------------" << endl;
	// vecInt.clear();		// 컨테이너 안에 있는 모든 원소를 삭제
	// 
	// if (vecInt.empty())	// 원소의 유무를 판단하는 함수
	// {
	// 	cout << "모두 삭제 완료" << endl;
	// 	cout << "사이즈 : " << vecInt.size() << "\t카파시티 : " << vecInt.capacity() << endl;
	// }

	// size : 컨테이너 안에 저장된 원소의 개수를 반환

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

	// front : 원소 중 맨 앞의 원소
	//cout << vecTemp.front() << endl;

	// back : 원소 중 맨 뒤의 원소
	//cout << vecTemp.back() << endl;

	vecTemp.front() = 100;
	vecTemp.back()  = 400;

	for (size_t i = 0; i < vecTemp.size(); ++i)
		 	cout << vecTemp[i] << endl;


	//vecTemp.clear();
	//
	//cout << "vecTemp 사이즈 : " << vecTemp.size() << "\t vecTemp 카파시티 : " << vecTemp.capacity() << endl;
	//
	//// vector<int>().swap(vecTemp);
	//// vecTemp.shrink_to_fit();
	//
	//cout << "vecTemp 사이즈 : " << vecTemp.size() << "\t vecTemp 카파시티 : " << vecTemp.capacity() << endl;


	// vector<int>		vecSrc;
	// 
	// vecSrc.push_back(100);
	// vecSrc.push_back(200);
	// vecSrc.push_back(300);
	// vecSrc.push_back(400);
	// vecSrc.push_back(500);
	// vecSrc.push_back(600);
	// 
	// cout << "vecTemp 사이즈 : " << vecTemp.size() << "\t vecTemp카파시티 : " << vecTemp.capacity() << endl;
	// cout << "vecSrc 사이즈 : " << vecSrc.size() << "\t vecSrc카파시티 : " << vecSrc.capacity() << endl;
	// 
	// vecTemp.swap(vecSrc);
	// 
	// cout << "======================after=============================" << endl;
	// 
	// cout << "vecTemp 사이즈 : " << vecTemp.size() << "\t vecTemp카파시티 : " << vecTemp.capacity() << endl;
	// cout << "vecSrc 사이즈 : " << vecSrc.size() << "\t vecSrc카파시티 : " << vecSrc.capacity() << endl;

	return 0;
}