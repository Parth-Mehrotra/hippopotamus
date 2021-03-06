#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

#include "header.h"

/* Create a new hashtable. */
hashTablePtr createTable(int size){

	hashTablePtr hashtable = NULL;
	int i;

	if( size < 1 )
		return NULL;

	/* Allocate the table itself. */
	if( ( hashtable = malloc( sizeof( hashTableT ) ) ) == NULL ) {
		return NULL;
	}

	/* Allocate pointers to the head nodes. */
	if( ( hashtable->table = malloc( sizeof( hashNodePtr ) * size ) ) == NULL ) {
		return NULL;
	}

	for( i = 0; i < size; i++ ) {
		hashtable->table[i] = NULL;
	}

	hashtable->size = size;
	hashtable->numEntries = 0;
	hashtable->numUniqueEntries = 0;

	return hashtable;	
}

/* Hash a string for a particular hash table. */
unsigned int hash(hashTablePtr hashtable, char *str)
{
    unsigned int hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % hashtable->size;
}

// Create new hashNode
hashNodePtr createHashNode(char* key) {
	hashNodePtr temp;

	if( ( temp = malloc( sizeof( hashNodePtr ) ) ) == NULL ) {
		return NULL;
	}

	if( ( temp->key = strdup( key ) ) == NULL ) {
		return NULL;
	}

	temp->ptr = NULL;
	temp->next = NULL;

	return temp;
}

char* parseFilePath(char* filePath)
{
	if(filePath == NULL)
		return NULL;
	int slashIndex = -1;
	int i = 0;
	for(i = 0; i < strlen(filePath); i++)
	{
		if(*(filePath+i) == '/')
			slashIndex = i;
	}
	if(slashIndex != -1)
		return filePath+slashIndex+1;
	return filePath;
}

/* Insert a key-value pair into a hash table. */
void insert(hashTablePtr hashtable, char* key, char* filePath) {
	char* fileName = parseFilePath(filePath);

	int bin = 0;
	hashNodePtr temp = NULL;
	hashNodePtr next = NULL;
	hashNodePtr prev = NULL;

	bin = hash(hashtable, key);
	bin = hash(hashtable, key);

	next = hashtable->table[bin];

	while(next != NULL && next->key != NULL && strcmp(key, next->key) > 0 ) {
		prev = next;
		next = next->next;
	}

	/* There's already a pair.  Let's replace that string. */
	if( next != NULL && next->key != NULL && strcmp(key, next->key) == 0 ) {
		SLInsert(next, fileName);
		hashtable->numEntries++;

	// Create new hashNode 
	} else {
		temp = createHashNode(key);

		if(next == hashtable->table[bin])
		{
			temp->next = next;
			hashtable->table[bin] = temp;
		} 
		else if (next == NULL)
		{
			prev->next = temp;
		}
		else
		{
			temp->next = next;
			prev->next = temp;
		}
		SLInsert(temp, fileName);
		hashtable->numUniqueEntries++;
		hashtable->numEntries++;
	}
}

/* Retrieves sortedList of  */
NodePtr getListOfFiles(hashTablePtr hashtable, char* key) {
	if(hashtable == NULL || key == NULL)
		return NULL;

	int bin = 0;
	hashNodePtr temp = NULL;
	hashNodePtr next = NULL;
	hashNodePtr prev = NULL;

	bin = hash(hashtable, key);
	bin = hash(hashtable, key);

	next = hashtable->table[bin];

	while(next != NULL && next->key != NULL && strcmp(key, next->key) > 0 ) {
		prev = next;
		next = next->next;
	}

	/* There's already a pair.  Let's replace that string. */
	if( next != NULL && next->key != NULL && strcmp(key, next->key) == 0 ) {
		return next->ptr;

	// Create new hashNode 
	} else {
		return NULL;
	}
}


void quickSort(char** a, int l, int r)
{
	int j;

	if(l < r) {
		j = partition(a, l, r);
		quickSort(a, l, j-1);
		quickSort(a, j+1, r);
	}
}

int partition(char** a, int l, int r) {
	int i = 0, j = 0;
	char* pivot = NULL;
	char* t = NULL;
	pivot = a[l];
	i = l; j = r+1;

	while(1) {
		do ++i; while(i <= r && strcmp(a[i] , pivot) <= 0);
		do --j; while(strcmp(a[j], pivot) > 0);
		if(i >= j) break;
		t = a[i];
		a[i] = a[j]; 
		a[j] = t;
	}
	t = a[l]; a[l] = a[j]; a[j] = t;
	return j;
}

char** getSortedList(hashTablePtr hashtable) {
	if(hashtable == NULL)
		return NULL;
	char** list = (char**) malloc(sizeof(char*) * hashtable->numUniqueEntries);
	memset(list, '\0', hashtable->numUniqueEntries);
	int i = 0, listIndex = 0;
	for(i = 0; i < hashtable->size; i++) {
		hashNodePtr temp = hashtable->table[i];
		while(temp != NULL) {
			list[listIndex] = temp->key;
			listIndex++;
			temp = temp->next;
		}
	}
	quickSort(list, 0, hashtable->numUniqueEntries - 1);
	return list;
}
