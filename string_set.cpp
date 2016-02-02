#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "string_set.h"

using namespace std;

string_set::string_set() {

	// Set all default values to NULL
	for(int i=0; i<HASH_TABLE_SIZE; i++) {
		hash_table[i] = NULL;
	}
	iterator_node = NULL;
	iterator_index = 0;
}

void string_set::add(const char *s) {

	// Check to see if s exists in the list already
	if( contains(s) == 1 ) {
		throw duplicate_exception();
	}

	// If not, create new node
	node *n = new node();
	char *d = new char[strlen(s)];
	(*n).s = strcpy(d, s);

	if ((n == NULL) || (d == NULL)) {
		throw memory_exception();
	}
	
	int tableGoto = hash_function(s);
	if (hash_table[tableGoto] == NULL) {
		// If hash field is empty, fill with pointer to new node.
		hash_table[tableGoto] = n;		
	}
	else
	{
		// Otherwise, move to last element in list, and point it towards new node.
		node *curr = hash_table[tableGoto];
		hash_table[tableGoto] = n;
		(*n).next = curr;		
	}
	
}

void string_set::remove(const char *s) {

	// Check to see if s exists in the list
	if( contains(s) == 0 ) {
		throw not_found_exception();
	}

	int tableGoto = hash_function(s);
	node *prev = NULL;
	node *curr = hash_table[tableGoto];

	// Scan each element of the current hash index
	// When the string is found, point the previous element to the next element 
	do {
		if (strcmp(s,(*curr).s) == 0) {
			node *next = (*curr).next;			
			if (prev == NULL) 
				hash_table[tableGoto] = next;
			else
				(*prev).next = next;
			break;
		}
		prev = curr;
	}
	while ( (curr = (*curr).next) != NULL );
	
	// Free memory associated with removed element
	delete (*curr).s;
	delete curr;
	reset();
}

int string_set::contains(const char *s) {
	
	int tableGoto = hash_function(s);

	// If hash field is empty, string s does not exist.
	if (hash_table[tableGoto] == NULL) {
		return 0;
	}
	else
	// Otherwise, scan through all elements in hash_table[x], if/until node.s equates with s
	{
		node *curr = hash_table[tableGoto];
		do {
			if (strcmp(s,(*curr).s) == 0) {
				return 1;
			}
		}
		while ( (curr = (*curr).next) != NULL );			
	}

	return 0;
}

void string_set::reset() {

	// Reset iterator
	iterator_index = 0;
	iterator_node = hash_table[0];
}

const char *string_set::next() {

	// If the iterator is not pointing towards anything, move to next element
	if (iterator_node != NULL)
		iterator_node = (*iterator_node).next;
	
	// If iterator is still not pointing towards anything, advance through the hash table
	// If last hash table index is passed, return null
	while ((iterator_node == NULL) && (iterator_index!=HASH_TABLE_SIZE-1)) {
			iterator_index++;
			iterator_node = hash_table[iterator_index];		
	}

	if (iterator_node == NULL)
		return NULL;
	else
		return (*iterator_node).s;
}

string_set::~string_set() {
	
	// Iterate through every hash table index and remove all free all elements contained within said index
	for( int i=0; i<HASH_TABLE_SIZE; i++) {
		while(hash_table[i] != NULL) {
			node *curr = hash_table[i];
			node *next = (*curr).next;
			delete (*curr).s;
			delete curr;
			hash_table[i] = next;
		}
	}
}

int string_set::hash_function(const char *s) {

	// Add ASCII values of all characters in s, and modulo the sum by HASH_TABLE_SIZE
	int sum = 0;
	int n = strlen(s);
	for( int i=0; i<n; i++ ) {
		sum = (sum+s[i])%HASH_TABLE_SIZE;	
	}

	return sum; 
}
