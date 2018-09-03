#include "LinkedList.h"
#include <iostream>

using namespace std;

//Note: the variable named current is used when iterating through the list.

/*
Constructor
Makes an empty linked list
*/

LinkedList::LinkedList()
{
	head = NULL;
	current = head;
}

/*
Constructor.
Takes in a node as an argument, and makes that node the head of the list.
*/

LinkedList::LinkedList(Node &node)
{
	head = &node;
	current = head;
}
//Creates a node using an integer argument, and inserts it at top of list.

void LinkedList::insert(int data)
{
	Node * node = new Node(data);
	reset();
	if (head == NULL)
	{
		head = node;
	} else {
		Node * temp = head;
		node->next = temp;
		head = node;
	}
}

//Returns the data associated with the node being pointed to by 'current'.

int LinkedList::getData()
{
	return current->data;
}

/*void LinkedList::update(int toUpdate)
{
	remove(toUpdate);
	insert(toUpdate);
}*/

//Iterates to the end of the list, and removes the last node on the list, and returns it.

Node * LinkedList::remove()
{
	reset();
	
	while (current->next->next != NULL)
	{
		current = current->next;
	}
	Node * returnVal = current->next;
	current->next = NULL;

	return returnVal;
}

//Searches for the node containing the value of the integer passed in as an argument, then removes it from the linked list and returns that node.

Node * LinkedList::remove(int toRemove)
{
	reset();
	Node * temp = NULL;

	while (current != NULL)
	{

		if (current->data == toRemove)
		{
			if (current == head)
			{
				head = current->next;
				current->next = NULL;
				return current;
			}
			temp->next = current->next;
			return current;
		} else {
			temp = current;
			current = current->next;
		}
	}
	return NULL;
}

//Same functionality as insert function, but takes a Node as an argument.

void LinkedList::pushFront(Node * newHead) {
	reset();
	Node * oldHead = head;
	newHead->next = oldHead;
	head = newHead;
}

//Sets the 'current' variable so it's pointing at the first node in the linked list.

void LinkedList::reset()
{
	current = head;
}

//Sets the 'current' variable to the next node in the linked list. If there is no node after the node the 'current' variable is pointing to, it returns false and does not change the 'current' varible.

bool LinkedList::next()
{
	if (current->next != NULL)
	{
		current = current->next;
		return true; 
	} else {
		return false;
	}
}

//Creates a node with its value equivalent to the integer argument.

Node::Node(int data)
{
	this->data = data;
	next = NULL;
}
