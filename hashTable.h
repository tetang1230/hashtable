/*
 *  * hashTable.h
 *   *
 *    *  Created on: Nov 30, 2011
 *     *      Author: Liam Q
 *      */
#ifndef HASHTABLE_H_
#define HASHTABLE_H_

//定义DEBUG_MSG用于调试输出
//#define DEBUG_MSG
#define DEBUG_MSG(args)	printf args;	printf("\n");

typedef char * ElemType;//对字符串进行hash

typedef struct ListNode//链表中节点
{
	ElemType elem;
	struct ListNode * next;
}ListNode, *Position;

typedef Position List;//链表

typedef struct HashTbl//hash表
{
	int tableSize;
	List * theLists;
}HashTbl, *HashTable;

HashTable initTable(int tableSize);
void destroyTable(HashTable hashTable);
Position find(HashTable hashtable, ElemType elem);
void insert(HashTable hashTable, ElemType elem);

#endif /* HASHTABLE_H_ */
