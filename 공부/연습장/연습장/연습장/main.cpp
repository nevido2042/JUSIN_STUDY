#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef struct tagPC
{
	vector<int> vecAdj;
	bool		isInfected = { false };
}PC;

int main()
{
	int iN = { 0 };
	int iM = { 0 };
	cin >> iN >> iM;

	vector<PC> vecPC(iN);
	for (int i = 0; i < iM; ++i)
	{
		int iPC1 = { 0 }, iPC2 = { 0 };
		cin >> iPC1 >> iPC2;
	}
}
