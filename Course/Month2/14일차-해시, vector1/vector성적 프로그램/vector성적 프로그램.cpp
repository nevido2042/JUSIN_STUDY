// vector성적 프로그램.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <vector>
using namespace std;
// vector 를 이용하여 성적표 프로그램 만들기
// 1. 추가 2. 출력 3. 검색 4. 삭제 5. 종료

class CStudent
{
    char m_szName[32];
    int m_iKor;
    int m_iEng;
    int m_iMat;
    float m_iAvg;
public:
    inline char* GetName()
    {
        return m_szName;
    }
    inline void Set_Name()
    {
        cin >> m_szName;
    }
    inline void Set_Kor(int _iScore)
    {
        m_iKor = _iScore;
    }
    inline void Set_Eng(int _iScore)
    {
        m_iEng = _iScore;
    }
    inline void Set_Mat(int _iScore)
    {
        m_iMat = _iScore;
    }
    inline void Calculate_Avg()
    {
        m_iAvg = m_iKor + m_iEng + m_iMat / 3.f;
    }
    inline void Print()
    {
        cout << "이름: " << m_szName << endl;
        cout << "국어: " << m_iKor << endl;
        cout << "영어: " << m_iEng << endl;
        cout << "수학: " << m_iMat << endl;
        cout << "평균: " << m_iAvg << endl;
    }
};

class CStudentManager
{
    vector<CStudent> m_vecStudent;
public:
    void Add();
    void Print();
    void Search();
    void Remove();
};

int main()
{
    enum { Add = 1, Print, Search, Remove, Exit };

    CStudentManager StudentManager;

    int iInput(0);

    while (true)
    {
        system("cls");
        cout << "1. 추가 2. 출력 3. 검색 4. 삭제 5. 종료" << endl;

        cin >> iInput;

        switch (iInput)
        {
        case Add:
            StudentManager.Add();
            break;
        case Print:
            StudentManager.Print();
            break;
        case Search:
            StudentManager.Search();
            break;
        case Remove:
            StudentManager.Remove();
            break;
        case Exit:
            return 0;
            break;
        default:
            continue;
            break;
        }
    }
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.

void CStudentManager::Add()
{
    system("cls");
    CStudent NewStudent;
    int iScore(0);

    cout << "이름: ";
    NewStudent.Set_Name();
    
    cout << "국어: ";
    cin >> iScore;
    NewStudent.Set_Kor(iScore);
    cout << "영어: ";
    cin >> iScore;
    NewStudent.Set_Eng(iScore);
    cout << "수학: ";
    cin >> iScore;
    NewStudent.Set_Mat(iScore);

    NewStudent.Calculate_Avg();

    m_vecStudent.push_back(NewStudent);
}

void CStudentManager::Print()
{
    system("cls");

    for (size_t i = 0; i < m_vecStudent.size(); ++i)
    {
        m_vecStudent[i].Print();
        cout << endl;
    }
    system("pause");
}

void CStudentManager::Search()
{
    system("cls");
    
    cout << "찾으려는 학생의 이름: ";
    char szFindName[32];
    cin >> szFindName;

    for (vector<CStudent>::iterator iter = m_vecStudent.begin(); iter != m_vecStudent.end(); ++iter)
    {
        if (strcmp(szFindName, (*iter).GetName()) == 0)
        {
            (*iter).Print();
            cout << endl;
        }
    }

    system("pause");
}

void CStudentManager::Remove()
{
    system("cls");

    cout << "삭제하려는 학생의 이름 : ";
    char szFindName[32];
    cin >> szFindName;

    for (vector<CStudent>::iterator iter = m_vecStudent.begin(); iter != m_vecStudent.end(); ++iter)
    {
        if (strcmp(szFindName, (*iter).GetName()) == 0)
        {
            iter = m_vecStudent.erase(iter);

            if (iter == m_vecStudent.end())
            {
                break;
            }
        }
    }

    system("pause");
}
