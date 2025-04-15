 #pragma once

enum class Color
{
	Red = 0,
	Black = 1,
};

struct Node
{
	Node*	parent = nullptr;
	Node*	left = nullptr;
	Node*	right = nullptr;
	int		key = {};
	Color	color = Color::Black;
};

// Red-Black Tree
// 1) 모든 노드는 Red / Black
// 2) Root는 Black
// 3) Leaf(NIL)는 Black
// 4) Red 노드의 자식은 Black (연속해서 Red-Red X)
// 5) 각 노드로부터 ~ 리프까지 가는 경로들은 모두 같은 수의 Black
class RedBlackTree
{
public:
	RedBlackTree();
	~RedBlackTree();

	// === 출력 ===
	void	Print() { Print(_root, 10, 0); }
	void	Print(Node* node, int x, int y);
	// root부터 순회 시작
	void	Print_Inorder() { Print_Inorder(_root); }
	void	Print_Inorder(Node* node);

	// === 작동 ===

	// 재귀 함수로 찾기
	Node*	Search(Node* node, int key);
	// 무한 루프로 찾기
	Node*	Search2(Node* node, int key);

	Node*	Min(Node* node);
	Node*	Max(Node* node);
	Node*	Next(Node* node);

	void	Insert(int key);
	void	InsertFixup(Node* node);

	void	Delete(int key);
	void	Delete(Node* node);

	void	Replace(Node* u, Node* v);

	// === Red-Black Tree 전용 함수 ===
	void	LeftRotate(Node* node);
	void	RightRotate(Node* node);

private:
	Node*	_root = nullptr;
	Node*	_nil = nullptr;
};

