
/***********************|
| Author: Andrew Morato |
| G Number: G00892138   |
| CS 463 - Project 2    |
| March 30, 2018        |
| linked_list.c         |
|***********************/

/*The purpose of this class is to provide a clean way to keep track
(not actually hold) each thread. Each thread (with its ID), alters
its corresponding element in the linked list when it has finished
executing. A link list was chosen over other data structures, for
the quick and effective way of removing elements at any position*/

#include "linked_list.h"

/*------------------------memory management------------------------*/

/*creates a head for the linked list, returns a pointer to the head*/
struct node * init_list() {return create_node(HEAD_VALUE);}

/*frees the head's next pointer and the head itself, all other
elements have already been freed by remove_completed()*/
void destroy_list(struct node * head){
	free(head->next);
	free(head);
}

/*returns a pointer to a new node with the given thread ID*/
struct node * create_node(int threadID){
	struct node * n = malloc(sizeof(struct node));
	n->threadID = threadID;
	n->thread_completed = 0;
	n->next = NULL;
	return n;
}

/*-------------------------list management-------------------------*/

/*finds and returns the tail of the list beginning at head*/
struct node * find_tail(struct node * head){
	struct node * p = head;
	int i = 0;
	while(p->next != NULL) {p = p->next;}
	return p;
}

/*adds a new node with the given threadID to the end of the list
beginning at head*/
void append_node(struct node * head, int threadID){
	struct node * n = create_node(threadID);
	struct node * tail = find_tail(head);
	tail->next = n;
}

/*returns a nonzero if the list beginning at head is nonempty*/
int is_empty(struct node * head){
	if(head->next == NULL) {return 1;}
	return 0;
}

/*prints the list (each node) beginning at the given head*/
void print(struct node * head){
	struct node * p = head->next;
	printf("Thread (Player) List\n");
	printf("ThreadID:  ");
	while(p != NULL){
		printf("%d ", p->threadID);
		p = p->next;
	}
	p = head->next;
	printf("\nCompleted: ");
	while(p != NULL){
		printf("%d ", p->thread_completed);
		p = p->next;
	}
	printf("\n\n");
}

/*-------------------------list application------------------------*/

/*finds a node matching the given threadID (if any), and sets
the thread_completed flag to THREAD_COMPLETED*/
void set_completed(struct node * head, int threadID){
	struct node * p = head;
	while(p != NULL) {
		/*sets the thread to completed and exits*/
		if(p->threadID == threadID){
			p->thread_completed = THREAD_COMPLETED;
			return;
		}
		p = p->next;
	}
}

/*removes the first node with a thread_completed flag set to 1
in the list beginning at head. Returns that node's threadID.
Returns -1 if there is no thread with a completed flag set*/
int remove_completed(struct node * head){
	struct node * p = head;
	while(p->next != NULL) {
		if(p->next->thread_completed == THREAD_COMPLETED){
			struct node * temp = p->next;
			int threadID = temp->threadID;
			p->next = temp->next;
			free(temp);
			return threadID;
		}
		p = p->next;
	}
	/*checks the tail of the list (uncheck in the loop above)*/
	if(p->thread_completed == THREAD_COMPLETED){
		int threadID = p->threadID;
		p = NULL; //sets itself to null
		free(p);
		return threadID;
	}
	return -1;
}