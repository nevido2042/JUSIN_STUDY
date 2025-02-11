﻿// 이진트리.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include "AVLTree.h"

#ifdef _DEBUG            // 메모리 누수 체크 매크로

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // 메모리 누수 체크 명령어

	//CNode<int>* Node1 = new CNode<int>;
	//CNode<int>* Node2 = new CNode<int>;
	//CNode<int>* Node3 = new CNode<int>;
	//CNode<int>* Node4 = new CNode<int>;

	//Node1->Set_Data(1);
	//Node2->Set_Data(2);
	//Node3->Set_Data(3);
	//Node4->Set_Data(4);

	//Node1->Set_Left(Node2);
	//Node1->Set_Right(Node3);
	//Node2->Set_Left(Node4);

	////cout << Node1->Get_Data() << endl;
	////cout << Node1->Get_Left()->Get_Data() << endl;
	////cout << Node1->Get_Right()->Get_Data() << endl;
	////cout << Node1->Get_Left()->Get_Left()->Get_Data() << endl;

	//Node1->Preorder_Traverse(&CNode<int>::Print_Data);
	//cout << endl;
	//Node1->Inorder_Traverse(&CNode<int>::Print_Data);
	//cout << endl;
	//Node1->Posorder_Traverse(&CNode<int>::Print_Data);

	////Node1->Posorder_Traverse(&CNode<int>::Delete_Tree);
	//Node1->Delete_Tree();

	/*Safe_Delete(Node1);
	Safe_Delete(Node2);
	Safe_Delete(Node3);
	Safe_Delete(Node4);*/

	/*CBinarySearchTree<int> BinarySearchTree;
	BinarySearchTree.Insert(2);
	BinarySearchTree.Insert(3);
	BinarySearchTree.Insert(1);

	BinarySearchTree.Preorder_Traverse(CNode<int>::Print_Data);
	cout << endl;
	BinarySearchTree.Inorder_Traverse(CNode<int>::Print_Data);
	cout << endl;
	BinarySearchTree.Postorder_Traverse(CNode<int>::Print_Data);
	cout << endl;
	CNode<int>* SearchNode;
	SearchNode = BinarySearchTree.Search(1);
	if (SearchNode)
	{
		cout << SearchNode->Get_Data() << endl;
	}
	else
	{
		cout << "탐색 실패" << endl;
	}
	SearchNode = BinarySearchTree.Search(99);
	if (SearchNode)
	{
		cout << SearchNode->Get_Data() << endl;
	}
	else
	{
		cout << "탐색 실패" << endl;
	}*/

	/*CBinarySearchTree<int> BinarySearchTree;
	BinarySearchTree.Insert(2);
	BinarySearchTree.Insert(3);
	BinarySearchTree.Insert(1);
	BinarySearchTree.Insert(4);
	BinarySearchTree.Insert(5);
	BinarySearchTree.Insert(6);

	BinarySearchTree.Inorder_Traverse(CNode<int>::Print_Data);
	cout << endl;

	CNode<int>* pNode = BinarySearchTree.Remove(4);
	Safe_Delete(pNode);
	BinarySearchTree.Inorder_Traverse(CNode<int>::Print_Data);*/

	CAVLTree<int> AVLTree;

	AVLTree.Insert(1);
	AVLTree.Insert(2);
	AVLTree.Insert(3);
	AVLTree.Insert(4);
	AVLTree.Insert(5);
	AVLTree.Insert(6);

	AVLTree.Inorder_Traverse(CNode<int>::Print_Data);
	cout << endl;

	delete AVLTree.Remove(3);

	AVLTree.Inorder_Traverse(CNode<int>::Print_Data);

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
