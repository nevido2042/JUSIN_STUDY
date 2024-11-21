#include "pch.h"

int main()
{
	array<int, 5> IntArr;
	IntArr[0] = 1;
	IntArr.at(2) = 1;

	map<int, string> StringMap;
	StringMap.insert({ 1,string("fdifjid") });
	StringMap.emplace(3, string("fjdifd"));
	StringMap.emplace(5, string("fjdifd"));
	StringMap.emplace(2, string("fjdifd"));

	set<string> StringSet;
	StringSet.emplace(string("fd"));

	forward_list<int> Int_FwdList;
	Int_FwdList.emplace_front(1);
	Int_FwdList.push_front(2);

	vector<int> vecInt{ 1,2,3,4,5 };

	std::sort(IntArr.begin(), IntArr.end());
	Int_FwdList.sort();
	//sort(StringMap.begin(), StringMap.end());
	
	for (auto iter = StringMap.begin(); iter != StringMap.end(); ++iter)
	{
		cout << iter->first << "\t" << iter->second << endl;
	}

	for (map<int,string>::iterator iter = StringMap.begin(); iter != StringMap.end(); ++iter)
	{
		cout << iter->first << "\t" << iter->second << endl;
	}

	cout << StringMap[3] << endl;

	for (int i : Int_FwdList)
	{
		cout << i << endl;
	}

	return 0;
}