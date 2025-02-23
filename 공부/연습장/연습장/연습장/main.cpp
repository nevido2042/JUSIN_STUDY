#include <iostream>
#include <vector>
using namespace std;

typedef struct tagInfo
{
	int iKg{};
	int iCm{};
	int iRate{};
}INFO;

int main()
{
	/*
	사람 받아와서 한사람과 다른사람 모두 비교 후 등급 매기기_O(N^2)
	*/
	int iN;
	cin >> iN;
	vector<INFO>vec(iN);
	for (int i = 0; i < iN; ++i)
	{
		cin >> vec[i].iKg;
		cin >> vec[i].iCm;
	}

	for (INFO& tINFO : vec)
	{
		int iRate{ 1 };
		for (const INFO& tOther : vec)
		{
			if (tINFO.iKg < tOther.iKg && tINFO.iCm < tOther.iCm)
			{
				++iRate;
			}
		}

		tINFO.iRate = iRate;
	}

	for (const INFO& tInfo : vec)
	{
		cout << tInfo.iRate << '\n';
	}
}