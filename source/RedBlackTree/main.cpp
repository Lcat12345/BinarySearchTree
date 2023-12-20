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

int main()
{
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

	bstInt.erase(123);

	cout << "메인함수 종료\n";
}