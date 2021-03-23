
/***********************|
| Author: Andrew Morato |
| G Number: G00892138   |
| CS 463 - Project 2    |
| March 30, 2018        |
| linked_list.h         |
|***********************/

/*------Preprocessor Directives------*/

#include <stdlib.h>
#include <stdio.h>

/*---------------Macros--------------*/

#define HEAD_VALUE -1
#define THREAD_COMPLETED 1

/*--------------Structs--------------*/

struct node {
	int threadID;
	/*marks if the thread has completed execution*/
	int thread_completed;
	struct node * next;
};

/*--------Function Prototypes--------*/

//Memory Management
struct node * init_list();
void destroy_list(struct node *);
struct node * create_node(int);

//List Management
struct node * find_tail(struct node *);
void append_node(struct node *, int);
int is_empty(struct node *);
void print(struct node *);

//List Application
void set_chair_taken(struct node *, int, int);
void set_completed(struct node *, int);
int remove_completed(struct node *);