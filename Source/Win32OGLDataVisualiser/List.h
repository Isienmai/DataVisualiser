#pragma once

template <class SelectedType> 
struct Node
{
	SelectedType content;
	Node *previousNode, *nextNode;
	Node() : content(), previousNode(nullptr), nextNode(nullptr){}
	Node(SelectedType _content, Node *_previousNode, Node *_nextNode) : content(_content), previousNode(_previousNode), nextNode(_nextNode) {}
};

template <class SelectedType> 
class LinkedList
{
	Node<SelectedType> *startNodeAddress, *endnode;
	int length;

public:
	LinkedList();
	LinkedList(SelectedType startNodeContents);
	~LinkedList();
	void DestroyMe();
	void RebootMe();
	void AddItem(SelectedType item);
	//Deletes the itemNum'th Node(0base)
	void RemoveItem(int itemNum);
	//Returns the itemNum'th Node Contents(0base)
	SelectedType GetItem(int itemNum) const;

	void SetItem(int itemnum, SelectedType replacementData);
	//Returns length of list
	int GetLength() const;
	//Returns last node number
	int GetFinalNodeNumber() const;
	//Moves node[itemNum] to node[0]
	void MoveToTop(int itemNum);
};

template <class SelectedType>
LinkedList<SelectedType>::LinkedList()
{
	endnode = new Node<SelectedType>(SelectedType(), nullptr, nullptr);
	startNodeAddress = endnode;
	length = 0;
}

template <class SelectedType>
LinkedList<SelectedType>::LinkedList(SelectedType startNodeContents)
{
	LinkedList();
	AddItem(startNodeContents);
}


template <class SelectedType>
LinkedList<SelectedType>::~LinkedList()
{		
}

template <class SelectedType>
void LinkedList<SelectedType>::DestroyMe()
{
	Node<SelectedType> *tempAddress = startNodeAddress;
	while (startNodeAddress != endnode)
	{
		startNodeAddress = tempAddress->nextNode;
		delete(tempAddress);
		tempAddress = startNodeAddress;
	}
	delete endnode;
}

template <class SelectedType>
void LinkedList<SelectedType>::AddItem(SelectedType item)
{
	if (startNodeAddress != endnode)
	{
		Node<SelectedType> *tempAddress = startNodeAddress;
		while (tempAddress->nextNode != endnode)
		{
			tempAddress = tempAddress->nextNode;
		}
		tempAddress->nextNode = new Node<SelectedType>(item, tempAddress, endnode);
	}
	else
	{
		startNodeAddress = new Node<SelectedType>(item, nullptr, endnode);
	}

	length++;
}

template <class SelectedType>
void LinkedList<SelectedType>::RemoveItem(int itemNum)
{
	Node<SelectedType> *tempPointer = startNodeAddress;

	if (itemNum != 0)
	{
		int counter = 0;

		while (counter != itemNum)
		{
			counter++;
			tempPointer = tempPointer->nextNode;
		}

		tempPointer->nextNode->previousNode = tempPointer->previousNode;
		tempPointer->previousNode->nextNode = tempPointer->nextNode;
		delete(tempPointer);
	}
	else
	{
		startNodeAddress->nextNode->previousNode = startNodeAddress;
		startNodeAddress = startNodeAddress->nextNode;
		delete(tempPointer);
	}

	length--;
}

template <class SelectedType>
SelectedType LinkedList<SelectedType>::GetItem(int itemNum) const
{
	if (itemNum != GetLength())
	{
		Node<SelectedType> *tempPointer = startNodeAddress;
		int counter = 0;

		while (counter != itemNum)
		{
			counter++;
			tempPointer = tempPointer->nextNode;
		}

		return tempPointer->content;
	}
	else
	{
		return SelectedType();
	}
}

template <class SelectedType>
void LinkedList<SelectedType>::SetItem(int itemNum, SelectedType replacementData)
{
	if (itemNum != GetLength())
	{
		Node<SelectedType> *tempPointer = startNodeAddress;
		int counter = 0;

		while (counter != itemNum)
		{
			counter++;
			tempPointer = tempPointer->nextNode;
		}

		tempPointer->content = replacementData;
	}
}

template <class SelectedType>
int LinkedList<SelectedType>::GetLength()  const
{
	return length;
}

template <class SelectedType>
int LinkedList<SelectedType>::GetFinalNodeNumber() const
{
	return length - 1;
}

template <class SelectedType>
void LinkedList<SelectedType>::MoveToTop(int itemNum)
{
	Node<SelectedType> *tempPointer = startNodeAddress;

	if (itemNum != 0)
	{
		int counter = 0;

		while (counter != itemNum)
		{
			counter++;
			tempPointer = tempPointer->nextNode;
		}

		tempPointer->previousNode->nextNode = tempPointer->nextNode;
		if (tempPointer->nextNode != endnode)
		{
			tempPointer->nextNode->previousNode = tempPointer->previousNode;
		}		
		tempPointer->nextNode = startNodeAddress;
		startNodeAddress->previousNode = tempPointer;
		startNodeAddress = tempPointer;
		startNodeAddress->previousNode = nullptr;
	}
}
