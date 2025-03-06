#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <list>
#include <map>
#include <stack>
#include <queue>
using namespace std;

int main()
{
	/*
	
	뽑을 수 Numbers[i]
	넣을 수 int iNum {1};

	while(true)
	{
		if (스택 비었나?)
		{
			수 넣기 (iNum++) (+)
		}
		else
		{
		
			if(탑이 iNum보다 크다)
			{
				NO
				break;
			}

			if(iNum와 탑이 같다.)
			{
				POP 해라. (-)
			}
			else
			{
				수 넣기 (iNum++) (+)
			}
		}

		if(iNum이 iN 보다 커지면)
			break;
	}
	*/

	int iN;
	cin >> iN;

	vector<int> Numbers(iN);

	for (int i = 0; i < iN; ++i)
	{
		cin >> Numbers[i];
	}

	stack<int> stack;
	list<char> PushPops;
	
	int iIndex{ 0 };
	int iNum{ 1 };

	while (true)
	{
		if (stack.empty())
		{
			stack.push(iNum++);
			PushPops.push_back('+');
		}
		else
		{
			if (stack.top() > Numbers[iIndex])
			{
				cout << "NO";
				return 0;
			}
			else if (stack.top() == Numbers[iIndex])
			{
				stack.pop();
				PushPops.push_back('-');
				++iIndex;
			}
			else
			{
				stack.push(iNum++);
				PushPops.push_back('+');
			}
		}

		if (iNum > iN)
			break;
	}

	while(!stack.empty())
	{
		if (stack.top() == Numbers[iIndex++])
		{
			stack.pop();
			PushPops.push_back('-');
		}
		else
		{
			cout << "NO";
			return 0;
		}
	}

	for (char c : PushPops)
	{
		cout << c << '\n';
	}
}
