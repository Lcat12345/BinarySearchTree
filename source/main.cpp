#include <iostream>
#include <map>
#include <string>

#include "BST.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

using std::map;
using std::cout;

class Delete
{
public:
	~Delete()
	{
		_CrtDumpMemoryLeaks();
		cout << "소멸했습니다!\n";
	}
};

Delete a;

int main()
{
	{
		map<int, std::string> m;
		
		m.insert(std::make_pair(20, "hi"));

		BST<int, std::string> bst;
		std::string temp = "hi";
		bst.insert(make_myPair(20, temp));
		temp = "hello";
		bst.insert(make_myPair(30, temp));
		temp = "kkk";
		bst.insert(make_myPair(4, temp));
		temp = "qqq";
		bst.insert(make_myPair(123, temp));
		temp = "www";
		bst.insert(make_myPair(99, temp));
		temp = "eee";
		bst.insert(make_myPair(12, temp));
		temp = "rrrr";
		bst.insert(make_myPair(44, temp));

		for (BST<int, std::string>::iterator Iter = bst.begin(); Iter != bst.end(); ++Iter)
		{
			cout << Iter->first << " " << Iter->second << "\n";
		}
	}


	static Delete d;

	BST<int, int> bstInt;

	bstInt.insert(make_myPair(100, 0));
	bstInt.insert(make_myPair(150, 0));
	bstInt.insert(make_myPair(25, 0));
	bstInt.insert(make_myPair(9, 0));
	bstInt.insert(make_myPair(200, 0));
	bstInt.insert(make_myPair(75, 0));
	bstInt.insert(make_myPair(40, 0));
	bstInt.insert(make_myPair(88, 0));
	bstInt.insert(make_myPair(123, 0));
	bstInt.insert(make_myPair(11, 0));
	bstInt.insert(make_myPair(101, 0));
	bstInt.insert(make_myPair(125, 0));
	bstInt.insert(make_myPair(127, 0));

	BST<int, int>::iterator bstIter;
	bstIter = bstInt.find(150);

	for (BST<int, int>::iterator Iter = bstInt.begin(); Iter != bstInt.end(); ++Iter)
	{
		cout << Iter->first << " " << Iter->second << "\n";
	}


	bstInt.erase(123);
	bstInt.erase(9);

	for (BST<int, int>::iterator Iter = bstInt.begin(); Iter != bstInt.end(); ++Iter)
	{
		cout << Iter->first << " " << Iter->second << "\n";
	}

	cout << "메인함수 종료\n";
}