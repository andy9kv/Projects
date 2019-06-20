 
Andrew Morato
Language: C (pthreads & locks)
March 30, 2018

NOTE: To run this project: 
   $ make
   $ ./p2 7 <----- (number is optional, default 10)
   $ make clean (command when finished, removes executable)

A musical chair game where the players are all threads racing to
grab one of the chairs (locks). An emcee (announcer) regulates this
process, such as turning the music on or off (i.e begin grabbing
chairs, stop, etc). 

   The music is represented by a integer, acting as a boolean flag,
   that is visible to all threads. The emcee sets it to 1 or 0 as
   needed to begin/end the rounds. Chairs are represented by a list
   of locks (technically structs that hold locks), each thread races
   to acquire them, randomly accesing the list and trying the lock.

   The strategy was to send each thread to randomly try chairs
   (locks) until the thread detected that all locks were taken.
   Theoretically, since access to the list of chairs is random,
   some chairs could never be taken. With enough iterations, it
   it essentailly is guaranteed that all chairs will be taken by
   the threads at some point. Once the final thread detected that
   all locks were taken (locked), it exited. Upon a thread exiting,
   a flag is set for the emcee to begin clean up, then a new round. 

   My biggest challange was the communitcation between the threads
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