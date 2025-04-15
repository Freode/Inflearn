#include "RedBlackTree.h"
#include <iostream>
#include <windows.h>

using namespace std;

enum class ConsoleColor
{
	BLACK = 0,
	RED = FOREGROUND_RED,
	GREEN = FOREGROUND_GREEN,
	BLUE = FOREGROUND_BLUE,
	YELLOW = RED | GREEN,
	WHITE = RED | GREEN | BLUE,
};


void SetCursorColor(ConsoleColor color)
{
	HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
	::SetConsoleTextAttribute(output, static_cast<SHORT>(color));
}

void SetCursorPosition(int x, int y)
{
	// 마우스 커서 위치 변경하기
	HANDLE output = ::GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
	::SetConsoleCursorPosition(output, pos);
}


RedBlackTree::RedBlackTree()
{
	_nil = new Node(); // Black
	_root = _nil;
}

RedBlackTree::~RedBlackTree()
{
	delete _nil;
}

// 트리 구조로 출력
// y : 몇 번째 줄
void RedBlackTree::Print(Node* node, int x, int y)
{
	if (node == _nil)
		return;

	SetCursorPosition(x, y);
	
	// 검은색은 안 보여서 파란색으로 임시 출력 설정
	if (node->color == Color::Black)
		SetCursorColor(ConsoleColor::BLUE);
	else
		SetCursorColor(ConsoleColor::RED);
	
	cout << node->key;
	Print(node->left, x - (5 / (y + 1)), y + 1);
	Print(node->right, x + (5 / (y + 1)), y + 1);

	SetCursorColor(ConsoleColor::WHITE);
}

// 데이터 출력
void RedBlackTree::Print_Inorder(Node* node)
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
Node* RedBlackTree::Search(Node* node, int key)
{
	if (node == _nil || key == node->key)
		return node;

	if (key < node->key)
		return Search(node->left, key);
	else
		return Search(node->right, key);
}

// 무한 루프로 찾기
// 재귀함수보다 스택 프레임을 적게 사용하므로 성능적으로 우월
Node* RedBlackTree::Search2(Node* node, int key)
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

// BinarySearch랑 다른 점 -> null check 부분을 _nil인지로 확인
Node* RedBlackTree::Min(Node* node)
{
	// 왼쪽 노드가 존재할 경우, 이동
	while (node->left != _nil)
		node = node->left;

	// 오른쪽 노드는 항상 자신보다 크므로, 무시

	return node;
}

Node* RedBlackTree::Max(Node* node)
{
	// 오른쪽 노드가 존재할 경우, 이동
	while (node->right != _nil)
		node = node->right;

	// 왼쪽 노드는 항상 자신보다 작으므로, 무시

	return node;
}

Node* RedBlackTree::Next(Node* node)
{
	// 자신보다 큰 다음 수
	// 1. 항상 자신보다 크므로 오른쪽으로 한 칸 이동
	// 2. 오른쪽 노드들 중에서 가장 왼쪽에 있는 노드가 자신보다 다음으로 큰 수

	// c.f) 가장 큰 수가 입력되면, 더 이상 탐색을 진행하지 않음.

	// 위 1번, 2번 과정을 수행
	if (node->right != _nil)
		return Min(node->right);

	// 더 이상 아래의 노드가 없는 경우
	// 부모 노드의 값이 자신보다 크므로, 다음으로 큰 수는 부모 노드의 값

	// 단, 현재 노드가 부모의 오른쪽 노드인 경우
	// 부모 노드의 값이 자신보다 작으므로, 조부모 노드로 올라가야 함.
	// 반복해서 올라갔을 때, 더 이상 현재 노드가 오른쪽이 아니라 왼쪽 노드인 경우
	// 그 부모의 노드가 자신보다 큰 값으로 설정됨.

	Node* parent = node->parent;

	// 부모 노드로 올라가서 오른쪽 노드로 이동
	while (parent != _nil && node == parent->right)
	{
		// 현재 노드 -> 부모 노드
		// 부모 노드 -> 조부모 노드
		node = parent;
		parent = parent->parent;
	}

	return parent;
}

void RedBlackTree::Insert(int key)
{
	Node* newNode = new Node();
	newNode->key = key;

	//if (_root == nullptr)
	//{
	//	_root = newNode;
	//	return;
	//}

	Node* node = _root;
	Node* parent = _nil;

	// key 데이터를 넣은 노드의 부모 노드 찾기
	while (node != _nil)
	{
		parent = node;

		if (key < node->key)
			node = node->left;

		else
			node = node->right;
	}

	// 부모-자식 구조 대입
	newNode->parent = parent;

	if (parent == _nil)
		_root = newNode;

	else if (key < parent->key)
		parent->left = newNode;

	else
		parent->right = newNode;

	// Red-Black Tree
	// ==== 추가 검사하는 코드 필요 =====
	newNode->left = _nil;
	newNode->right = _nil;
	newNode->color = Color::Red; // 새로운 노드는 항상 빨간색으로 시작

	// 새롭게 추가한 노드가 규칙을 잘 지키고 있는지 확인
	InsertFixup(newNode);
}

void RedBlackTree::InsertFixup(Node* node)
{
	// 규칙이 3가지 존재 -> 3가지의 경우가 존재
	// 1) p = red, uncle = red
	// -> p = black, uncle = black, pp = red로 바꿈
	//
	// 2) p = red, unclde = black (triangle)
	// -> 회전을 통해서 case 3으로 바꿔줌
	// 
	// 3) p = red, uncle = black (list)
	// -> 색상 변경 + 회전

	// uncle node = 부모의 부모 노드 중 반대쪽 자식
	//		[pp]
	//	[p]		[u]
	//	  [n]

	// 수정이 필요 없는 상황
	//	[p(B)]
	//		[n(R)]

	// Red-Black Tree 규칙의 4번 위배 -> 색상 보정 필요
	//	[p(R)]
	//		[n(R)]

	// parent의 color가 red일 때까지 무한 루프
	while (node->parent->color == Color::Red)
	{
		//		[pp(B)]
		//	[p(R)]	[u(?)]
		//		[n(R)]
		if (node->parent == node->parent->parent->left)
		{
			Node* uncle = node->parent->parent->right;

			// uncle node에 대한 검사도 원래는 필요하지만,
			// 모든 노드에 대해서 _nil(더미 노드)로 넣어 놨기 때문에,
			// nullptr이면, 검정색으로 판단됨.
			
			// 1) p = red, uncle = red
			//		[pp(B)]			  [pp(R)]
			//	[p(R)]	[u(R)] -> [p(B)]	[u(B)]
			//		[n(R)]			[n(R)]
			// pp, p, u 색상만 바꿔치기
			if (uncle->color == Color::Red)
			{
				node->parent->color = Color::Black; // p
				uncle->color = Color::Black; // u
				node->parent->parent->color = Color::Red; // pp

				// pp노드까지 변경되었을 때, ppp와 비교해서 색상이 잘 지켜져 있는지 반복적으로 확인할 필요 있음.
				node = node->parent->parent;
			}

			// === triangle ===
			// pp & p & n 이 ㄱ자로 삼각형으로 이루고 있음
			//		[pp(B)]		
			//	[p(R)]	[u(B)]
			//		[n(R)]		
	
			// === list ===
			// pp & p & n 이 ㅡ자로 삼각형으로 이루고 있음
			//		[pp(B)]		
			//	[p(R)]	[u(B)]
			//[n(R)]		

			else
			{
				// 2) p = red, unclde = black (triangle)
				//		[pp(B)]			  [pp(R)]
				//	[p(R)]	[u(B)] -> [p(B)]	[u(B)]
				//		[n(R)]			[n(R)]
				// pp, p, u 색상만 바꿔치기
				if (node == node->parent->right) // triangle type
				{
					// 1. 왼쪽으로 list 형태로 펴주는 작업
					//		[pp(B)]				[pp(B)]
					//	[p(R)]	[u(B)] ->	[n(R)]		[u(B)]
					//		[n(R)]		  [p(R)]	
					
					// p 기준으로 왼쪽 회전
					node = node->parent;
					LeftRotate(node);

					// 그러면, p가 결국은 다시 n으로 바뀌게 됨.
					//		[pp(B)]
					//	[p(R)]		[u(B)]
					//[n(R)]	
					// -> case 3인 list 타입으로 변경 완료
				}

				// 3) p = red, uncle = black (list)
				// List 타입

				//		[pp(B)]
				//	[p(R)]		[u(B)]
				//[n(R)]

				//		[pp(R)]
				//	[p(B)]		[u(B)]
				//[n(R)]
				node->parent->color = Color::Black;
				node->parent->parent->color = Color::Red;

				// pp를 대상으로 right rotation 진행
				// red가 양 옆으로 등장하게끔 변경
				//		 [p(B)]
				//	[n(R)]	   [pp(R)]
				//					[u(B)]
				RightRotate(node->parent->parent);
			}

		}
		//		[pp(B)]
		//			[p(R)]
		//		[n(R)]
		else
		{
			Node* uncle = node->parent->parent->left;

			// 1) p = red, uncle = red
			//		[pp(B)]			  [pp(R)]
			//	[u(R)]	[p(R)] -> [u(B)]	[p(B)]
			//		   [n(R)]			  [n(R)]
			// pp, p, u 색상만 바꿔치기
			if (uncle->color == Color::Red)
			{
				node->parent->color = Color::Black; // p
				uncle->color = Color::Black; // u
				node->parent->parent->color = Color::Red; // pp

				// pp노드까지 변경되었을 때, ppp와 비교해서 색상이 잘 지켜져 있는지 반복적으로 확인할 필요 있음.
				node = node->parent->parent;
			}
			// triangle & list 타입 처리
			else
			{
				// 2) p = red, unclde = black (triangle)
				//		[pp(B)]			  [pp(R)]
				//	[u(R)]	[p(B)] -> [u(B)]	[p(B)]
				//		   [n(R)]			  [n(R)]
				// pp, p, u 색상만 바꿔치기
				if (node == node->parent->left) // triangle type
				{
					// 1. 오른쪽으로 list 형태로 펴주는 작업
					//		[pp(B)]				[pp(B)]
					//	[u(R)]	[p(B)] ->	[u(R)]	[n(B)]
					//		   [n(R)]		           [p(R)]	

					// p 기준으로 오른쪽 회전
					node = node->parent;
					RightRotate(node);

					// 그러면, p가 결국은 다시 n으로 바뀌게 됨.
					//		[pp(B)]
					//	[u(B)]	[p(R)]
					//              [n(R)]	
					// -> case 3인 list 타입으로 변경 완료
				}

				// 3) p = red, uncle = black (list)
				// List 타입

				//		[pp(B)] // fix
				//	[p(R)]		[u(B)]
				//[n(R)]

				//		[pp(R)] // fix
				//	[p(B)]		[u(B)]
				//[n(R)]
				node->parent->color = Color::Black;
				node->parent->parent->color = Color::Red;

				// pp를 대상으로 right rotation 진행
				// red가 양 옆으로 등장하게끔 변경
				//		 [p(B)]
				//	[pp(R)]	   [n(R)]
				//[u(B)]				
				LeftRotate(node->parent->parent);
			}
		}
	}

	_root->color = Color::Black;
}

// 가장 까다로움
void RedBlackTree::Delete(int key)
{
	// 삭제할 노드
	// 1) 자식이 없는 경우 -> 쉬움
	// 2) 자식이 1개 있는 경우 -> 비교적 쉬움
	// 3) 자식이 2개 있는 경우 -> 조금 머리 아픔

	Node* deleteNode = Search(_root, key);
	Delete(deleteNode);
}

void RedBlackTree::Delete(Node* node)
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
void RedBlackTree::Replace(Node* u, Node* v)
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

// 위 -> 아래 : 오른쪽 회전
// 아래 -> 위 : 왼쪽 회전

//	     [y]
//	  [x]   [3]
//  [1][2]

//	    [x]
//	 [1]   [y]
//        [2][3]

// 왼쪽으로 노드 회전
void RedBlackTree::LeftRotate(Node* x)
{
	// x(pp)에 대해서 왼쪽 회전
	Node* y = x->right;

	// == [x]와 [2]의 관계 수정 ==
	x->right = y->left; // [2]
	// 항상 존재한다고 보장 못함
	if(y->left != _nil)
		y->left->parent = x;

	// ppp 변경
	y->parent = x->parent;

	// x가 최상위 부모일 경우
	if (x->parent == _nil)
		_root = y;
	// x가 ppp의 왼쪽 노드일 경우
	else if (x == x->parent->left)
		x->parent->left = y;
	// x가 ppp의 오른쪽 노드일 경우
	else
		x->parent->right = y;

	// == [y]와 [x]의 관계 수정 == 
	y->left = x;
	x->parent = y;

}

// 위 -> 아래 : 오른쪽 회전
// 아래 -> 위 : 왼쪽 회전

//	     [y]
//	  [x]   [3]
//  [1][2]

//	    [x]
//	 [1]   [y]
//        [2][3]

// 오른쪽으로 노드 회전
void RedBlackTree::RightRotate(Node* y)
{
	// y(pp)에 대해서 오른쪽 회전
	Node* x = y->left;

	// == [y]와 [2]의 관계 수정 ==
	y->left = x->right;
	// 항상 존재한다고 보장 못함
	if (y->left != _nil) // [2]
		y->left->parent = y;

	// ppp 변경
	x->parent = y->parent;

	// y가 최상위 부모일 경우
	if (y->parent == _nil)
		_root = x;
	// y가 ppp의 왼쪽 노드일 경우
	else if (y == y->parent->left)
		y->parent->left = x;
	// y가 ppp의 오른쪽 노드일 경우
	else
		y->parent->right = x;

	// == [y]와 [x]의 관계 수정 == 
	x->right = y;
	y->parent = x;
}
