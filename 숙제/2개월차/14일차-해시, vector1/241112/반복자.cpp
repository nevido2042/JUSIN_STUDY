#include "pch.h"
#include <list>

int main()
{
	// 반복자(iterator) : 포인터와 비슷한 개념

	vector<int>		vecInt;

	vecInt.push_back(10);
	vecInt.push_back(20);
	vecInt.push_back(30);
	vecInt.push_back(40);

	vector<int>::iterator		iter = vecInt.begin();
	iter += 2;

	// 중간 삽입 함수
	// vecInt.insert(iter, 500);

	// 중간 삭제 함수

	iter = vecInt.erase(iter);		// erase 이후 다음 가리킬 대상을 반환
	cout << *iter << endl;

	// for (vector<int>::iterator	iter = vecInt.begin();
	// 	iter != vecInt.end();  ++iter)
	// {
	// 	cout << *iter << endl;
	// }
	

	// 1. 출력 반복자 : 컨테이너 내의 원소 읽기만 가능한 반복자
	// 2. 입력 반복자 : 컨테이너 내의 원소 읽기, 쓰기 모두 가능한 반복자
	// 3. 정방향(순방향) 반복자 : 컨테이너 내의 원소를 정방향으로만 이동하여 접근이 가능
	// 4. 양방향 반복자 : 컨테이너 내의 원소를 양방향으로 모두 이동하여 접근이 가능
	// 5. 임의 접근 반복자 : 컨테이너 내의 원소의 임의 접근을 허용

	return 0;
}

// vector 를 이용하여 성적표 프로그램 만들기
// 1. 추가 2. 출력 3. 검색 4. 삭제 5. 종료