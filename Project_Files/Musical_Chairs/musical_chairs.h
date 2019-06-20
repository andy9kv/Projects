
/***********************|
| Author: Andrew Morato |
| March 30, 2018        |
| musical_chairs.h      |
|***********************/

/*------Preprocessor Directives------*/

#include <pthread.h>
#include "linked_list.h"

/*--------------Structs--------------*/

/*represents a char, holding its ID and
whether or not it is taken. Holds a lock
to prevent other threads grabbing a taken
chair*/
struct chair{
	int chairID;
	int threadID;
	volatile int taken;
	pthread_mutex_t lock;
};

/*---------------Macros--------------*/

#define CHAIR_TAKEN 1
#define CHAIR_FREE 0
#define ALL_CHAIRS_TAKEN 0
#define ALL_CHAIRS_FREE 1
#define DEFAULT_PLAYERS 10

/*----------Global Variables---------*/

//Lists----

/*A list of chair "objects" to be competed for by threads*/
struct chair ** chairs;
/*A head pointer to a linked list to track players (threads)*/
struct node * players;

//Flags----

/*defines the number of chairs left*/
int remaining_chairs;
/*flag to notify the announcer (emcee) that the round is over.
Only set by the last player (thread) when it finds that all chairs
have already been taken, right before it eliminated itself (exits)*/
int round_over;
/*music control that regulates the starting/ending of each round*/
int music;

/*--------Function Prototypes--------*/

void emcee();
void* play_game(void *);
int chair_status();
void free_memory(int);
void print_info();

/*-------------pthreads--------------*/

/*
	(for reference)

	pthreads_mutex_lock(lock) - the calling thread acquires the lock
	if possible, its blocked (sleeps) until the lock is free if not

	pthreads_mutex_trylock(lock) - identical to lock() but returns 0
	if the lock has been acquired by the calling thread, nonzero if 
	not

	pthread_mutex_unlock(lock) - releases the lock, wakes up (sets to
	ready) any threads that have been blocked attempting to acquire
	the lock being released

	pthread_exit() - exits the current thread

	pthread_mutex_destroy(lock) - destorys the given lock
*/