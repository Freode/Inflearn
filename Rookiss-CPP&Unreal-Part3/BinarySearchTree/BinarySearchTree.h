#pragma once
struct Node
{
	Node*	parent = nullptr;
	Node*	left = nullptr;
	Node*	right = nullptr;
	int		key = {};
};

class BinarySearchTree
{
public:

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

	void	Delete(int key);
	void	Delete(Node* node);

	void	Replace(Node* u, Node* v);

private:
	Node*	_root = nullptr;
};

