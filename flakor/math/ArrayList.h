#ifndef _FK_X_H_
#define _FK_X_H_

FLAKOR_NS_BEGIN
#pragma once

template <typename T>
class ArrayList
{
	private:
 	//type node
 	struct ListNode
 	{
  		T data;
  		ListNode* next;
 	};
 	//basic variable
 	ListNode* head;
 	ListNode* point;
 	int count ;
 	//find an element
 	ListNode* find(T elem)
 	{
  		if (isEmpty)
   		return NULL;
  		ListNode* p = head->next;
  		while(true)
  		{
   			if (p->data == elem || p == NULL)
    		break;
   			else
   			{
    			p = p->next;
   			}
  }
  return p;
 }
 ListNode* findbyindex(int index)
 {
  int i = 0;
  ListNode* p = head->next;
  while(i != index)
  {
   p = p->next;
   ++i;
  }
  return p;
 }
public:
 //build a listarray
 ArratList <T> (void)
 {
  head = (ListNode*)malloc(sizeof(ListNode));
  point = (ListNode*)malloc(sizeof(ListNode));
  head->next = point;
  point->next = NULL;
  count = 0;
 }
 //distrebuit
 virtual ~ArratList <T> (void)
 {
  if(head)
   free(head);
 }
 //add a element at last space;
 void add(T elem)
 {
  point->data = elem;
  point->next = (ListNode*)malloc(sizeof(ListNode));
  point = point->next;
  point->next = NULL;
  ++count;
 }
 void add(T elem,int index)
 {
  if (index > length()-1)
  {
   printf("%s/n","Index is too long!");
   return;
  }
  ListNode* p = findbyindex(index-1);
  ListNode* n = (ListNode*)malloc(sizeof(ListNode));
  n->data = elem;
  n->next = p->next;
  p->next = n;
  ++count;
 }
 //return length
 int length()
 {
  return count;
 }
 //return whether the list is empty
 bool isEmpty()
 {
  if(head->next)
   return true;
  else
   return false;
 }
 //return if this element is the last one
 bool isLast(T elem)
 {
  ListNode* p = find(elem);
  if (p==NULL)
  {
   printf("%s/n","No this element!");
   return false;
  }
  if (p->next == point)
   return true;
  else
   return false;
 }
 //return an element
 T at(int index)
 {
  if (index > length()-1)
  {
   printf("%s/n","Index is too long!");
   return (T)NULL;
  }
  return findbyindex(index)->data;
 }
};
FLAKOR_NS_END
