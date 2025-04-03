#include "BinarySearchTree.h"
#include <iostream>
#include <windows.h>

using namespace std;

void SetCursorPosition(int x, int y)
{
	// 마우스 커서 위치 변경하기
	HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
	::SetConsoleCursorPosition(output, pos);
}


// 트리 구조로 출력
// y : 몇 번째 줄
void BinarySearchTree::Print(Node* node, int x, int y)
{
	if (node == nullptr)
		return;

	SetCursorPosition(x, y);
	cout << node->key;
	Print(node->left, x - (5 / (y + 1)), y + 1);
	Print(node->right, x + (5 / (y + 1)), y + 1);
}

// 데이터 출력
void BinarySearchTree::Print_Inorder(Node* node)
{
	// 어떤 방식을 통해서 데이터를 스캔해야 할까?

	// 1) 전위 순회(preorder traverse)
	// 2) 중위 순회(inorder traverse)
	// 3) 후위 순회(postorder traverse)

	if (node == nullptr)
		return;

	// 1) 전위 순회
	//cout << node->key << endl;
	//Print_Inorder(node->left);
	//Print_Inorder(node->right);

	// 2) 중위 순회
	Print_Inorder(node->left);
	cout << node->key << endl;
	Print_Inorder(node->right);

	// 3) 후위 순회
	//Print_Inorder(node->left);
	//Print_Inorder(node->right);
	//cout << node->key << endl;

}

// 재귀 함수로 찾기
// 무한 루프보다 직관적으로 가독성이 높음
Node* BinarySearchTree::Search(Node* node, int key)
{
	if (node == nullptr || key == node->key)
		return node;

	if (key < node->key)
		return Search(node->left, key);
	else
		return Search(node->right, key);
}

// 무한 루프로 찾기
// 재귀함수보다 스택 프레임을 적게 사용하므로 성능적으로 우월
Node* BinarySearchTree::Search2(Node* node, int key)
{
	while (node && key != node->key)
	{
		if (key < node->key)
			node = node->left;
		else
			node = node->right;
	}

	return node;
}

Node* BinarySearchTree::Min(Node* node)
{
	// 왼쪽 노드가 존재할 경우, 이동
	while (node->left)
		node = node->left;

	// 오른쪽 노드는 항상 자신보다 크므로, 무시

	return node;
}

Node* BinarySearchTree::Max(Node* node)
{
	// 오른쪽 노드가 존재할 경우, 이동
	while (node->right)
		node = node->right;

	// 왼쪽 노드는 항상 자신보다 작으므로, 무시

	return node;
}

Node* BinarySearchTree::Next(Node* node)
{
	// 자신보다 큰 다음 수
	// 1. 항상 자신보다 크므로 오른쪽으로 한 칸 이동
	// 2. 오른쪽 노드들 중에서 가장 왼쪽에 있는 노드가 자신보다 다음으로 큰 수

	// c.f) 가장 큰 수가 입력되면, 더 이상 탐색을 진행하지 않음.

	// 위 1번, 2번 과정을 수행
	if (node->right)
		return Min(node->right);

	// 더 이상 아래의 노드가 없는 경우
	// 부모 노드의 값이 자신보다 크므로, 다음으로 큰 수는 부모 노드의 값

	// 단, 현재 노드가 부모의 오른쪽 노드인 경우
	// 부모 노드의 값이 자신보다 작으므로, 조부모 노드로 올라가야 함.
	// 반복해서 올라갔을 때, 더 이상 현재 노드가 오른쪽이 아니라 왼쪽 노드인 경우
	// 그 부모의 노드가 자신보다 큰 값으로 설정됨.

	Node* parent = node->parent;

	// 부모 노드로 올라가서 오른쪽 노드로 이동
	while (parent && node == parent->right)
	{
		// 현재 노드 -> 부모 노드
		// 부모 노드 -> 조부모 노드
		node = parent;
		parent = parent->parent;
	}

	return parent;
}

void BinarySearchTree::Insert(int key)
{
	Node* newNode = new Node();
	newNode->key = key;

	if (_root == nullptr)
	{
		_root = newNode;
		return;
	}

	Node* node = _root;
	Node* parent = nullptr;

	// key 데이터를 넣은 노드의 부모 노드 찾기
	while (node)
	{
		parent = node;

		if (key < node->key)
			node = node->left;

		else
			node = node->right;
	}

	// 부모-자식 구조 대입
	newNode->parent = parent;

	if (key < parent->key)
		parent->left = newNode;

	else
		parent->right = newNode;
}

// 가장 까다로움
void BinarySearchTree::Delete(int key)
{
	// 삭제할 노드
	// 1) 자식이 없는 경우 -> 쉬움
	// 2) 자식이 1개 있는 경우 -> 비교적 쉬움
	// 3) 자식이 2개 있는 경우 -> 조금 머리 아픔

	Node* deleteNode = Search(_root, key);
	Delete(deleteNode);
}

void BinarySearchTree::Delete(Node* node)
{
	if (node == nullptr)
		return;

	// 자식이 없거나, 1개인 경우
	if (node->left == nullptr)
		// 자식이 없으면, nullptr로 대체
		// 자식이 오른쪽에만 있으면, 해당 노드와 대체
		Replace(node, node->right);

	// 자식이 왼쪽에만 존재
	else if (node->right == nullptr)
		Replace(node, node->left);

	// 자식이 둘 다 존재할 때
	else
	{
		// 다음 데이터 찾기
		Node* next = Next(node);

		// 다음 데이터를 기존 데이터에 넣음으로써,
		// 처음부터 원래 해당 데이터인 것처럼 작동
		node->key = next->key;
		Delete(next);
	}
}

// u 서브트리를 v 서브트리로 교체
// 그리고 delete u
void BinarySearchTree::Replace(Node* u, Node* v)
{
	// u가 최상위 노드
	if (u->parent == nullptr)
		_root = v;

	// 상위 노드의 왼쪽 노드인 경우
	else if (u == u->parent->left)
		u->parent->left = v;

	// 상위 노드의 오른쪽 노드인 경우
	else
		u->parent->right = v;

	// v 노드의 부모를 u 노드의 부모로 변경
	// - v가 nullptr로 들어올 수 있는 경우도 존재
	// - 아예 끊어버릴 때, nullptr이 들어옴.
	if (v)
		v->parent = u->parent;

	delete u;
}
