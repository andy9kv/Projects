/*
 * Copyright (c) 2000, 2001, 2002, 2003, 2004, 2005, 2008, 2009
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
 * Synchronization primitives.
 * The specifications of the functions are in synch.h.
 */

#include <types.h>
#include <lib.h>
#include <spinlock.h>
#include <wchan.h>
#include <thread.h>
#include <current.h>
#include <synch.h>


////////////////////////////////////////////////////////////
//                                                        //
//                       Semaphore                        //
//                                                        //
////////////////////////////////////////////////////////////


struct semaphore *
sem_create(const char *name, unsigned initial_count)
{
        struct semaphore *sem;

        sem = kmalloc(sizeof(struct semaphore));
        if (sem == NULL) {
                return NULL;
        }

        sem->sem_name = kstrdup(name);
        if (sem->sem_name == NULL) {
                kfree(sem);
                return NULL;
        }

	sem->sem_wchan = wchan_create(sem->sem_name);
	if (sem->sem_wchan == NULL) {
		kfree(sem->sem_name);
		kfree(sem);
		return NULL;
	}

	spinlock_init(&sem->sem_lock);
        sem->sem_count = initial_count;

        return sem;
}

void
sem_destroy(struct semaphore *sem)
{
        KASSERT(sem != NULL);

	/* wchan_cleanup will assert if anyone's waiting on it */
	spinlock_cleanup(&sem->sem_lock);
	wchan_destroy(sem->sem_wchan);
        kfree(sem->sem_name);
        kfree(sem);
}

void
P(struct semaphore *sem)
{
        KASSERT(sem != NULL);

        /*
         * May not block in an interrupt handler.
         *
         * For robustness, always check, even if we can actually
         * complete the P without blocking.
         */
        KASSERT(curthread->t_in_interrupt == false);

	/* Use the semaphore spinlock to protect the wchan as well. */
	spinlock_acquire(&sem->sem_lock);
        while (sem->sem_count == 0) {
		/*
		 *
		 * Note that we don't maintain strict FIFO ordering of
		 * threads going through the semaphore; that is, we
		 * might "get" it on the first try even if other
		 * threads are waiting. Apparently according to some
		 * textbooks semaphores must for some reason have
		 * strict ordering. Too bad. :-)
		 *
		 * Exercise: how would you implement strict FIFO
		 * ordering?
		 */
		wchan_sleep(sem->sem_wchan, &sem->sem_lock);
        }
        KASSERT(sem->sem_count > 0);
        sem->sem_count--;
	spinlock_release(&sem->sem_lock);
}

void
V(struct semaphore *sem)
{
        KASSERT(sem != NULL);

	spinlock_acquire(&sem->sem_lock);

        sem->sem_count++;
        KASSERT(sem->sem_count > 0);
	wchan_wakeone(sem->sem_wchan, &sem->sem_lock);

	spinlock_release(&sem->sem_lock);
}


////////////////////////////////////////////////////////////
//                                                        //
//                         Lock.                          //
//                                                        //
////////////////////////////////////////////////////////////


struct lock *
lock_create(const char *name)
{
        struct lock *lock;

        lock = kmalloc(sizeof(struct lock));
        if (lock == NULL) {
                return NULL;
        }

        lock->lk_name = kstrdup(name);
        if (lock->lk_name == NULL) {
                kfree(lock);
                return NULL;
        }

	// Instantiates the added fields to their starting values
	lock->lk_holder = NULL;
        lock->lk_wchan = wchan_create(lock->lk_name);
	spinlock_init(&lock->lk_spinlock);

        return lock;
}

void
lock_destroy(struct lock *lock)
{
        KASSERT(lock != NULL);
	// assert that someone is not still holding the lock
	KASSERT(lock->lk_holder == NULL);

        // free the added fields
        wchan_destroy(lock->lk_wchan);
	spinlock_cleanup(&lock->lk_spinlock);

        kfree(lock->lk_name);
        kfree(lock);
}


/* The idea behind this lock implementation is to put threads that want
 * to acquire a taken lock to sleep on the lock's wait channel until the
 * thread that holds the lock releases it and wakes up the first sleeping
 * thread on the wait channel. wchan_sleep() and wchan_wakeone() take care
 * of ordering the sleeping threads and ensuring fairness (i.e. first thread
 * to request the lock gets it when the lock becomes free) */
void
lock_acquire(struct lock *lock)
{
        /* Asserts that the lock exists and that we (this thread) do
         * not already hold this lock */
	KASSERT(lock != NULL);
	KASSERT(!lock_do_i_hold(lock));

	/* Ensures an uninterrupted critical section and prepares the
 	 * spinlock to be passed to wchan_sleep if necessary */
	spinlock_acquire(&lock->lk_spinlock);

     	/* Begins a loop that drives the functionality of this lock.
 	 * First checks if the lock has a holder. If it does not, then
 	 * the lock is grabbed, the spinlock released, and the function
 	 * exits. However if the lock is already taken, the thread goes
 	 * to sleep on the lock's wait channel (back of the line), to be
 	 * awoken when it reaches the front of the queue and a thread
 	 * releases the lock (taken care of in lock_release() */
	while(1) {

		/* Checks if the lock is free, grabs it if it is*/
		if (lock->lk_holder == NULL) {
			lock->lk_holder = curthread; // grabs the free lock
			/* The only way out of this function is through here,
 			 * so the spinlock is guaranteed to be released */
			spinlock_release(&lock->lk_spinlock);
			return;
		}
		/* If the lock is not free, goes to sleep on the wchan*/
		else {
			// wchan needs a locked spinlock
			wchan_sleep(lock->lk_wchan, &lock->lk_spinlock);
		}
	}
}

/* The second half of the lock mechanism described above, this function is
 * quite simple, it just acquires the spinlock to ensure the critical
 * section is protected, wakes up the first sleeping thread on the lock's
 * wait channel, and releases the lock. In part, the spinlock is there to
 * prevent the recently woken up thread, or any thread, from jumping in
 * front, finding the lock taken, and going back to sleep. This would
 * create a deadlock, in a sense, since no thread will be there to wake
 * up any more sleeping threads */
void
lock_release(struct lock *lock)
{
	/* Asserts that this lock does, in fact, exist and that
 	 * we currently hold it, or else there would be no sense
 	 * in trying to release it! */
        KASSERT(lock != NULL); 
	KASSERT(lock_do_i_hold(lock));

	/* Grabs the spinlock to ensure a protected critical section
 	 * and to call wchan_wakeone() with our locked spinlock */
        spinlock_acquire(&lock->lk_spinlock);

	/* Wakes up the first thread sleeping on this lock's wchan*/
	wchan_wakeone(lock->lk_wchan, &lock->lk_spinlock);

	/* Releases the lock. As mentioned before, the spinlock
 	 * protects this from being abused and handled out of order */
	lock->lk_holder = NULL;

	/* Finally, releases the spinlock and gets out of here */
	spinlock_release(&lock->lk_spinlock);
}

/* Simply returns if this thread holds the lock or not */
bool
lock_do_i_hold(struct lock *lock)
{
        /* Asserts that this lock is not a figment of our imagination */
	KASSERT(lock != NULL);

	if(lock->lk_holder == curthread) { return true; }
	else { return false; }
}


////////////////////////////////////////////////////////////
//                                                        //
//                          CV.                           //
//                                                        //
////////////////////////////////////////////////////////////


struct cv *
cv_create(const char *name)
{
        struct cv *cv;

        cv = kmalloc(sizeof(struct cv));
        if (cv == NULL) {
                return NULL;
        }

        cv->cv_name = kstrdup(name);
        if (cv->cv_name==NULL) {
                kfree(cv);
                return NULL;
        }

        // add stuff here as needed

        return cv;
}

void
cv_destroy(struct cv *cv)
{
        KASSERT(cv != NULL);

        // add stuff here as needed

        kfree(cv->cv_name);
        kfree(cv);
}

void
cv_wait(struct cv *cv, struct lock *lock)
{
        // Write this
        (void)cv;    // suppress warning until code gets written
        (void)lock;  // suppress warning until code gets written
}

void
cv_signal(struct cv *cv, struct lock *lock)
{
        // Write this
	(void)cv;    // suppress warning until code gets written
	(void)lock;  // suppress warning until code gets written
}

void
cv_broadcast(struct cv *cv, struct lock *lock)
{
	// Write this
	(void)cv;    // suppress warning until code gets written
	(void)lock;  // suppress warning until code gets written
}
