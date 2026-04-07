#pragma once

#include "config.h"

typedef struct linknode {
	pos Pos[8];
	int order;
	struct linknode* next;
	struct linknode* prev;
	long long data[8];
}linknode;
void create_linklist(linknode* head);
void append_linknode(linknode* head, int xpos,int ord);
void delete_first_node(linknode* head);
int delete_first_node_if(linknode* head);