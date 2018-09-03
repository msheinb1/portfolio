#ifndef LINKEDLIST_H
#define LINKEDLIST_H

class Node
{
	public:
	int data;
	Node * next;

	public:
	Node(int data); //check
};

class LinkedList
{
	Node * head;
	Node * current;

	public:
	LinkedList();
	LinkedList(Node& node); //check

	void insert(int data); //Create
	int getData(); //Read
	//void update(); //Update
	Node * remove();
	Node * remove(int toRemove);
	void pushFront(Node * push);
	Node * getBack();
	void reset();
	bool next();
};
#endif
