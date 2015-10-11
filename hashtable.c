/*
 *  * hashTable.c
 *   *
 *    *  Created on: Nov 30, 2011
 *     *      Author: Liam Q
 *      */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "hashTable.h"

#define MIN_TABLE_SIZE 10//hash表最小大小

static int isPrime(int a)
{
	int i;
	int b = (int)sqrt((double)a);
	for(i = 2; i <= b; i++)
	  if(a % i == 0)
		return 0;
	return 1;
}

static int nextPrime(int a)
{
	while(!isPrime(a++));
	return --a;
}

HashTable initTable(int tableSize)
{
	HashTable hashTable;
	int i;

	if(tableSize < MIN_TABLE_SIZE)
	{
		printf("Table size is too small\n");
		return NULL;
	}

	hashTable = (HashTable)malloc(sizeof(HashTbl));
	if(hashTable == NULL)
	  exit(-1);
	hashTable->tableSize = nextPrime(tableSize);

	hashTable->theLists = (List *)malloc(hashTable->tableSize * sizeof(List));
	if(!hashTable->theLists)
	  exit(-1);

	for(i = 0; i < hashTable->tableSize; i++)
	{
		hashTable->theLists[i] = (List)malloc(sizeof(ListNode));
		if(!hashTable->theLists[i])
		  exit(-1);
		else
		  hashTable->theLists[i]->next = NULL;
	}

	return hashTable;
}

void destroyTable(HashTable hashTable)
{
	int i;
	List l, tmp;

	if(hashTable == NULL)
	  return;
	if(hashTable->theLists)
	{
		for(i = 0; i < hashTable->tableSize; i++)
		{
			l = hashTable->theLists[i];
			while(l != NULL)
			{
				tmp = l;
				l = l->next;
				free(tmp);
			}
			hashTable->theLists[i] = NULL;
		}
		free(hashTable->theLists);
	}

	free(hashTable);
}

static int hash(ElemType elem, int tableSize)
{
	unsigned int hashVal = 0;

	while(*elem != '\0')
	  hashVal = (hashVal << 5) + *elem++;

	return hashVal % tableSize;
}

Position find(HashTable hashTable, ElemType elem)
{
	Position p;
	List l;

	l = hashTable->theLists[hash(elem, hashTable->tableSize)];
	p = l->next;
	while(p != NULL && strcmp(p->elem, elem) != 0)
	  p = p->next;

	return p;
}

void insert(HashTable hashTable, ElemType elem)
{
	Position p, pNewCell;
	List l;

	p = find(hashTable, elem);
	//感觉这步,查找是多余的,真正的插入已经在下面else语句完成了
	if(p == NULL)
	{
		pNewCell = (List)malloc(sizeof(ListNode));
		if(pNewCell == NULL)
		  exit(-1);
		else
		{
			l = hashTable->theLists[hash(elem, hashTable->tableSize)];
			pNewCell->elem = (char *)malloc(strlen(elem) * sizeof(char));
			strcpy(pNewCell->elem, elem);
			pNewCell->next = l->next;
			l->next = pNewCell;
			//即便有hash冲突,也不走到最后的位置,直接在当前hash table的位置后面
			//将新元素添加进来。冲突的元素,排在新进来的元素后面
			//这样保证了insert的时间复杂度是o(1)
			DEBUG_MSG(("%d-----%s\n",hash(elem, hashTable->tableSize),pNewCell->elem));
		}
	}
}


/*
 *  * demo.c .利用上面实现的hash表读取文本文件中单词，并将其放入hash表
 *   * 
 *    *  Created on: Nov 30, 2011
 *     *      Author: Liam Q
 *      */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "hashTable.h"

#define HASH_SIZE 500
#define MAXWORD	64

void createHashTable(HashTable hashTable)
{
	char *fileName="words.txt";
	FILE * fp;
	char word[MAXWORD];
	char c;
	int i;

	if((fp = fopen(fileName, "r")) == NULL)
	{
		fprintf(stderr,"can't open %s\n",fileName);
		exit(-1);
	}

	c = ' ';
	while(!feof(fp))
	{
		while(c != EOF && isspace(c))
		  c = fgetc(fp);
		i = 0;
		while(c != EOF && !isspace(c))
		{
			word[i++] = c;
			c = fgetc(fp);
		}
		if(c == EOF)
		  break;
		word[i] = '\0';
		while(i >= 0 && ispunct(word[--i]))
		  word[i] = '\0';
		insert(hashTable,word);
	}
}

int main()
{
	HashTable hashTable;
	char word[MAXWORD];

	hashTable = initTable(HASH_SIZE);
	createHashTable(hashTable);
	while(1)
	{
		printf("Search string:");
		fgets(word, sizeof(word), stdin);
		word[strlen(word) - 1] = '\0';
		DEBUG_MSG(("%s",word));
		printf("%s\n",find(hashTable,word)?"find":"not find");
	}
	destroyTable(hashTable);
	hashTable = NULL;
}

