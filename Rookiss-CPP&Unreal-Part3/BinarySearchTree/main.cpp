#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include "RedBlackTree.h"
#include <thread>

using namespace std;



int main()
{
	RedBlackTree bst;

	bst.Insert(30);
	bst.Print();
	this_thread::sleep_for(1s);

	bst.Insert(10);
	bst.Print();
	this_thread::sleep_for(1s);

	bst.Insert(20);
	bst.Print();
	this_thread::sleep_for(1s);
	
	bst.Insert(25);
	bst.Print();
	this_thread::sleep_for(1s);

	bst.Insert(40);
	bst.Print();
	this_thread::sleep_for(1s);
	
	bst.Insert(50);
	bst.Print();
	this_thread::sleep_for(1s);

	// bst.Delete(20);

	// bst.Print();

	return 0;
}

