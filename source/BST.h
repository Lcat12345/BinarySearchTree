#pragma once
#include <memory>


using std::unique_ptr;

template <typename T1, typename T2>
struct myPair
{
	T1 first;
	T2 second;
};

//template <typename T1, typename T2>
//myPair<T1, T2>& make_myPair(const T1& _first, const T2& _second)
//{
//	myPair<T1, T2> pair { _first, _second};
//	return pair;
//}


template <typename T1, typename T2>
myPair<T1, T2> make_myPair(const T1& _first, const T2& _second)
{	
	return myPair<T1, T2> { _first, _second};
}


template <typename K, typename V>
struct Node
{
	myPair<K, V> pair;

	Node* parent;
	Node* LeftChild;
	Node* RightChild;
};

template <typename Key, typename Value>
class BST 
{
public:
	BST()
 		: RootNode(nullptr), nodeCount(0)
	{

	}

	~BST()
	{
		// post-order travel
		post_order_release(RootNode);
		//RootNode = nullptr;
	}

	void insert(const myPair<Key, Value>& _pair);
	void post_order_release(Node<Key, Value>* node);
	Node<Key, Value>* GetInOrderSuccessor(Node<Key, Value>* _Node);

	bool isLeft(Node<Key, Value>* curNode)
	{
		if (curNode->parent->LeftChild == curNode)
			return true;
		else
			return false;
	}

public:
	class iterator
	{
	public:
		friend class BST;

		iterator()
			: bst(nullptr),
			 curNode(nullptr)
		{}

		iterator(BST<Key, Value>* _bst, Node<Key, Value>* _node)
			: bst(_bst),
			  curNode(_node)
		{}

		/*
		* 오류 코드
		* iterator(const BST<Key, Value>& _bst, const Node<Key, Value>& _node)
			: bst(_bst),
			  curNode(_node)
		{}
		*/

		bool operator == (const iterator& _other)
		{
			if (bst == _other.bst && curNode == _other.curNode)
				return true;
			else
				return false;
		}

		bool operator != (const iterator& _other)
		{
			return !(*this == _other);
		}

		const myPair<Key, Value>* operator ->()
		{
			return &curNode->pair;
		}
					 
		iterator& operator ++()
		{
			curNode = bst->GetInOrderSuccessor(curNode);
			return *this;
		}



	private:
		BST<Key, Value>* bst;
		Node<Key, Value>* curNode;
	};

	iterator begin();
	iterator end();
	iterator find(const Key& first);
	bool erase(const Key& _key);

private:
	Node<Key, Value>* RootNode;
	int nodeCount;
};

template<typename Key, typename Value>
inline void BST<Key, Value>::insert(const myPair<Key, Value>& _pair)
{
	Node<Key, Value>* newNode = new Node<Key, Value>;

	newNode->pair = _pair;
	newNode->parent = nullptr;
	newNode->LeftChild = nullptr;
	newNode->RightChild = nullptr;

	Node<Key, Value>* pNode = RootNode;

	if (nullptr == pNode)
	{
		RootNode = newNode;
	}
	else
	{
		while (true)
		{
			if (pNode->pair.first > newNode->pair.first)
			{
				if (nullptr == pNode->LeftChild)
				{
					newNode->parent = pNode;
					pNode->LeftChild = newNode;
					break;
				}
				pNode = pNode->LeftChild;
			}
			else if (pNode->pair.first < newNode->pair.first)
			{
				if (nullptr == pNode->RightChild)
				{
					newNode->parent = pNode;
					pNode->RightChild = newNode;
					break;
				}
				pNode = pNode->RightChild;
			}
			else
			{
				return;
			}
		}	
	}

	nodeCount++;
}

template<typename Key, typename Value>
inline void BST<Key, Value>::post_order_release(Node<Key, Value>* node)
{
	if (nullptr != node)
	{
		post_order_release(node->LeftChild);
		post_order_release(node->RightChild);
		std::cout << ">>" << node->pair.first << "\n";
		delete node;
	}	
}

template<typename Key, typename Value>
inline Node<Key, Value>* BST<Key, Value>::GetInOrderSuccessor(Node<Key, Value>* _Node)
{
	Node<Key, Value>* Successor = nullptr;

	// 1. 오른쪽 자식이 있는 경우
	// 오른쪽 자식으로 간 후 왼쪽 자식 노드가 없을 때 까지 왼쪽 자식으로 내려감
	if (nullptr != _Node->RightChild)
	{
		Successor = _Node->RightChild;

		bool b = Successor->LeftChild;

		while (Successor->LeftChild)
		{
			Successor = Successor->LeftChild;
		}
	}
	// 2. 오른쪽 자식이 없는 경우
	// 부모가 후속자인데 현재 노드가 부모의 왼쪽 자식이여야 함.
	else
	{
		Successor = _Node;

		while (true)
		{
			// 계속 올라갔는데 루트 노드임 -> 후속자 없음 -> end iterator
			if (nullptr == Successor->parent)
			{
					return nullptr;
			}

			// 내가 부모의 왼쪽 자식이니?
			if (Successor->parent->LeftChild == Successor)
			{
				Successor = Successor->parent;
				break;
			}
			else
			{
				Successor = Successor->parent;
			}
		}

	}

	return Successor;
}

template<typename Key, typename Value>
inline typename BST<Key, Value>::iterator BST<Key, Value>::begin()
{
	Node<Key, Value>* pNode = RootNode;

	while (pNode->LeftChild)
	{
		pNode = pNode->LeftChild;
	}

	return iterator(this, pNode);
}

template<typename Key, typename Value>
inline typename BST<Key,Value>::iterator BST<Key, Value>::end()
{
	return iterator(this, nullptr);
}

template<typename Key, typename Value>
inline typename BST<Key, Value>::iterator BST<Key, Value>::find(const Key& first)
{
	Node<Key, Value>* pNode = RootNode;

	while (true)
	{
		if (pNode->pair.first > first)
		{
			if (nullptr == pNode->LeftChild)
			{
				// 못찾음
				pNode = nullptr;
				break;
			}
			pNode = pNode->LeftChild;
		}
		else if (pNode->pair.first < first)
		{
			if (nullptr == pNode->RightChild)
			{
				// 못찾음
				pNode = nullptr;
				break;
			}
			pNode = pNode->RightChild;
		}
		else
		{
			// 찾음
			break;
		}
	}

	return iterator(this, pNode);
}

template<typename Key, typename Value>
inline bool BST<Key, Value>::erase(const Key& _key)
{
	BST<Key, Value>::iterator iter = find(_key);

	if (end() == iter)
	{
		return false;
	}
	else
	{
		// 지우려는 노드가 단말 노드인 경우
		// 자식이 없으므로 부모 노드의 자식을 nullptr로 만들어줘야함.
		if (nullptr == iter.curNode->LeftChild && nullptr == iter.curNode->RightChild)
		{
			if (isLeft(iter.curNode))
				iter.curNode->parent->LeftChild = nullptr;
			else
				iter.curNode->parent->RightChild = nullptr;
						
			delete iter.curNode;
		}
		// 지우려는 노드가 단말 노드가 아닌 경우
		else
		{
			// 지워지는 노드는 지우려하는 노드의 후속자
			Node<Key, Value>* nextNode = GetInOrderSuccessor(iter.curNode);

			// 후속자가 자식이 없는 경우
			// 여기도 역시 자식이 없으므로 nullptr 넣는 것 주의
			if (nullptr == nextNode->LeftChild && nullptr == nextNode->RightChild)
			{
				iter.curNode->pair = nextNode->pair;

				if (isLeft(nextNode))
					nextNode->parent->LeftChild = nullptr;
				else
					nextNode->parent->RightChild = nullptr;

				delete nextNode;

			}
			// 후속자가 자식이 있는 경우
			else
			{
				iter.curNode->pair = nextNode->pair;

				// 데이터 복사 후 후속자 노드를 삭제해야 한다. 
				// 후속자가 자식이 있어서 자식과 후속자의 부모를 연결해줘야 한다.
				nextNode->RightChild->parent = nextNode->parent;

				// 후속자가 왼쪽 자식이면 후속자의 자식 노드를 후속자의 부모노드의 왼쪽 자식으로 연결
				// 아니면 그 반대
				if (isLeft(nextNode))
					nextNode->parent->LeftChild = nextNode->LeftChild;
				else
					nextNode->parent->RightChild = nextNode->RightChild;
					
				

				delete nextNode;
			}
		}
	}	

	return true;
}
