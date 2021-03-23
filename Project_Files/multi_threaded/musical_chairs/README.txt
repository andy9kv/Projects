 
Andrew Morato
G00892138
CS 463 - Project 2
Language: C (pthreads & locks)
March 30, 2018

NOTE: To run this project: 
   $ make
   $ ./p2 7 <----- (number is optional, default 10)
   $ make clean (command when finished, removes executable)

1. Structure Notes

	The music is represented by a integer, acting as a boolean flag,
	that is visible to all threads. The emcee sets it to 1 or 0 as
	needed to begin/end the rounds. Chairs are represented by a list
	of locks (technically structs that hold locks), each thread races
	to acquire them, randomly accesing the list and trying the lock.
	Locations, Lines:

		Turn music ON - musical_chairs.c, line 42 (on init), line 103
		Turn music OFF - musical_chairs.c, line 90
		Lock creation - musical_chairs.c, line 49

2. The strategy was to send each thread to randomly try chairs
   (locks) until the thread detected that all locks were taken.
   Theoretically, since access to the list of chairs is random,
   some chairs could never be taken. With enough iterations, it
   it essentailly is guaranteed that all chairs will be taken by
   the threads at some point. Once the final thread detected that
   all locks were taken (locked), it exited. Upon a thread exiting,
   a flag is set for the emcee to begin clean up, then a new round. 

3. My biggest challange was the communitcation between the threads
   and the emcee. The solution used (albeit not preferable) is
   the setting of flags (global ints) and a list of the threads,
   which does not hold the threads, but simply is there to keep track
   of them. The threads use their corresponding slots in the list to
   set if they have "lost". The emcee will use this list to figure
   out which thread has left, remove the corresponding entry, and
   continue with the game. Racing to acquire one of the locks was the
   competition of resouces, but I did not face any issues here. The
   "cooperation" was when the threads that were lucky enough to
   acquire a lock nicely busy-waited for the ones that were sill
   competing.

4. The laborous parts were mostly self-inflicted (linked list of
   threads problems, leaving temprorary code for too long which
   messed up the rest). The pthreads library in C was great to work
   with. As mentioned above, what took the longest by far was setting
   up a good system that allowed communication between the emcee and
   the threads.

5. The one bug that I faced, relating to the competition/cooperation
   of the threads, was with the threads with chairs waiting for the
   other threads that were still attempting to acquire chairs. The
   problem seemed to be that threads tried to grab multiple chairs,
   perhaps "moving on" to the next round prematurely. This was
   solved by making the threads (inefficiently) busy wait (in an
   empty while loop) until a flag, that marked the end of the round,
   was set. To debug these problems, I exclusivly used manual
   inspection and print statements. NOTE: A method that prints all
   variables in memory is avaiable (not currently used at all) in
   musical_chairs.c called print_info()
