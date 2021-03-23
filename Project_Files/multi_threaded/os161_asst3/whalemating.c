/*
 * Copyright (c) 2001, 2002, 2009
 *	The President and Fellows of Harvard College.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * Driver code for whale mating problem
 */
#include <types.h>
#include <lib.h>
#include <thread.h>
#include <test.h>
#include <synch.h>

// Constants
#define NMATING 10

// Global Synchronization Primatives

/* Locks for the whaless that are next in line to mate */
struct lock * male_lock;
struct lock * female_lock;
struct lock * mm_lock;

// Other Global Primitives
volatile int ready;      // whates increment this when they are ready to mate
volatile int mated;      // whales increment this when they have mated
volatile int checked_in; // whales increment this when they are ready
volatile int exited;     // whales increment this when they leave 
volatile long maleID;
volatile long femaleID;
volatile long mmID;

/* ------ Functions ------ */

static
void
male(void *p, unsigned long which)
{
	(void)p;
	kprintf("[male whale #%ld starting]\n", which);

	lock_acquire(male_lock); 

	// ----------------------------------------------------------------
	
	maleID = which;             // sets the male ID for others to see
	ready += 1;                 // announces the whale is ready
	while (ready != 3) {}       // waits until all whales are ready
	while (checked_in != 2) {}  // waits for the other whales
	ready = 0;             	    // resets the ready variable
	checked_in = 0;             // resets the checked_in in variable 

	// mates
	kprintf("[male #%ld mated with female #%ld, per matchmaker #%ld]\n", 
	         maleID, femaleID, mmID); 
	mated += 1;

	// waits until everyone in the trio has finished their mating
	// business before leaving
	while (mated != 3) {}
	while (exited != 2) {}
	mated = 0;                // resets the mated variable
	exited = 0;               // resets the exited variable
	
	lock_release(male_lock); 

	// ----------------------------------------------------------------

	// exits the thread
	thread_exit();
}


static
void
female(void *p, unsigned long which)
{
	(void)p;
	kprintf("[female whale #%ld starting]\n", which);

	lock_acquire(female_lock); 

	// ----------------------------------------------------------------
	
	femaleID = which;           // sets the female ID for others to see
	ready += 1;                 // announces the whale is ready
	while (ready != 3) {}       // waits until all whales are ready
	checked_in += 1;            // declares it has checked in

	// mates
	kprintf("[female #%ld mated with male #%ld, per matchmaker #%ld]\n", 
	         femaleID, maleID, mmID); 
	mated += 1;

	// waits until everyone in the trio has finished their mating
	// business before leaving
	while (mated != 3) {}
	exited += 1;              // announces that it has exited it's wait
	
	lock_release(female_lock); 

	// ----------------------------------------------------------------

	// exits the thread
	thread_exit();
}

static
void
matchmaker(void *p, unsigned long which)
{
	(void)p;
	kprintf("[matchmaker whale #%ld starting]\n", which);

	lock_acquire(mm_lock); 

	// ----------------------------------------------------------------
	
	mmID = which;               // sets the matchmaker ID for others to see
	ready += 1;                 // announces the whale is ready
	while (ready != 3) {}       // waits until all whales are ready
	checked_in += 1;            // declares it has checked in

	// mates
	kprintf("[matchmaker #%ld has overseen male #%ld and female #%ld mate]\n", 
	         mmID, maleID, femaleID); 
	mated += 1;

	// waits until everyone in the trio has finished their mating
	// business before leaving
	while (mated != 3) {}
	exited += 1;              // announces that it has exited it's wait
	
	lock_release(mm_lock); 

	// ----------------------------------------------------------------

	// exits the thread
	thread_exit();
}


// Change this function as necessary
int
whalemating(int nargs, char **args)
{

	/* ------ Initializes Globals ------ */

	// initializes the locks
	male_lock = lock_create("male lk");
	female_lock = lock_create("female lk");
	mm_lock = lock_create("matchmaker lk");

	// initializes the int flags
	ready = 0;
	mated = 0;
	exited = 0;
	checked_in = 0;
	maleID = 0;
	femaleID = 0;
	mmID = 0;

	/* --------------------------------- */


int i, j, err=0;

(void)nargs;
(void)args;

for (i = 0; i < 3; i++) {
	for (j = 0; j < NMATING; j++) {
		switch(i) {
		    case 0:
			err = thread_fork("Male Whale Thread",
					  NULL, male, NULL, j+1);
			break;
		    case 1:
			err = thread_fork("Female Whale Thread",
					  NULL, female, NULL, j+1);
			break;
		    case 2:
			err = thread_fork("Matchmaker Whale Thread",
					  NULL, matchmaker, NULL, j+1);
			break;
		}
		if (err) {
			panic("whalemating: thread_fork failed: %s)\n",
			      strerror(err));
		}

	}
}
thread_exit();
//return 0;
}
