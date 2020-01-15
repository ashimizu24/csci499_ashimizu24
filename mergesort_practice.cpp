#include <iostream>
#include "mergesort.h"
using namespace std;
 
int main()
{
	Node* end = new Node(8);
	Node* fourth = new Node(2);
	fourth->next = end;
	Node* third = new Node(7);
	third->next = fourth;
	Node* second = new Node(1);
	second->next = third;
	Node* first = new Node(9);
	first->next = second;
	Node* head = new Node(3);
	head->next = first;

	// for(Node* node = head; node != NULL; node = node->next){
	// 	cout << node->data << "\n";
	// }
	split(head, 6);

    return 0;
}