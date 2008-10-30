/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2008 Ascent Team <http://www.ascentemu.com/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef RWLOCK_H
#define RWLOCK_H

#include "Condition.h"
//#include "Mutex.h"
/*
class RWLock
{
public: 
  
	ASCENT_INLINE void AcquireReadLock()
	{
		//_lock.Acquire();
		_cond.BeginSynchronized();
		_readers++;
		//_lock.Release();
		_cond.EndSynchronized();
	}
	
	ASCENT_INLINE void ReleaseReadLock()
	{
		//_lock.Acquire();
		_cond.BeginSynchronized();
		if(!(--_readers))
			if(_writers)
				_cond.Signal();
		//_lock.Release();
		_cond.EndSynchronized();
	}

	ASCENT_INLINE void AcquireWriteLock()
	{
		//_lock.Acquire();
		_cond.BeginSynchronized();
		_writers++;
		if(_readers)
			_cond.Wait();
		//assert(_readers==0);
	}

	ASCENT_INLINE void ReleaseWriteLock()
	{
		if(--_writers)
			_cond.Signal();
		//_lock.Release();
		_cond.EndSynchronized();
	}
	ASCENT_INLINE RWLock() : _cond(&_lock) {_readers=0;_writers=0;}
  
	private:
		FullMutex _lock;
		Condition _cond;
		volatile unsigned int _readers;
		volatile unsigned int _writers;
   
}; */

#ifdef WIN32

class RWLock
{
	/**
	 * XXX: This works on the basis that for each time SetEvent is called
	 *      it will "wake up" one thread that is waiting on it.
	 */

	/**
	 * Concept: Multiple threads can read data at once, only one thread can 
	            write data at once.

				AcquireReadLock will lock the main mutex, and increment the
				reader count by one, then release the mutex.

				ReleaseReadLock will lock the main mutex, decrement the reader
				count by one. If there are writers waiting, it will signal them
				to wake up. Upon waking up, the writer threads will attempt to lock
				the main mutex. After they have this lock, they can check if there
				are still readers present (that means that the CPU switched to another
				thread which happened to grab the read lock before it could switch back
				with the critical section locked), if there are readers still present,
				it has to wait for the event again.

				AcquireWriteLock is detailed above.
				ReleaseWriteLock releases the main mutex, and decrements the writer count
				by one.
				
				*NOTE* this *may* still be possible to a race condition,
				however it will only mean that it may take longer for a "write"
				operation to gain ownership of a lock than a "read" operation, no
				clashes of threads mangling data.
	*/

	// event used to "wake up" writer threads
	HANDLE hWriterWakeup;

	// synchronization object used to control access to below resources
	CRITICAL_SECTION hLock;

	// stored threads
	volatile long _writers;
	volatile long _readers;

public:

	RWLock() : _writers(0), _readers(0)
	{
		// initialize writer wakeup event
		hWriterWakeup = CreateEvent(NULL, FALSE, FALSE, NULL);

		// initialize main lock
		InitializeCriticalSection(&hLock);
	}

	~RWLock()
	{
		// resources should all be released at this point
		assert(_writers == 0);
		assert(_readers == 0);

		// delete wakeup event
		CloseHandle(hWriterWakeup);

		// delete main lock
		DeleteCriticalSection(&hLock);
	}

	void AcquireReadLock()
	{
		// acquire the main lock, increment reader count by one, 
		// release the main lock, allowing other threads to call this function and stack up

		EnterCriticalSection(&hLock);

		// add this thread
		++_readers;

		LeaveCriticalSection(&hLock);
	}

	void ReleaseReadLock()
	{
		// acquire main lock
		EnterCriticalSection(&hLock);

		// are we the last reading thread?
		if( (--_readers) <= 0 )
		{
			// wake up one of the writing threads, if there are any queued
			if( _writers > 0 )
				SetEvent(hWriterWakeup);
		}

		// release main lock
		LeaveCriticalSection(&hLock);
	}

	void AcquireWriteLock()
	{
		// acquire main lock
		EnterCriticalSection(&hLock);

		// increment writer count
		++_writers;

		for(;;)
		{
			// check if there are any active readers
			if( _readers > 0 )
			{
				// sorry, have to wait until the read operations complete
				LeaveCriticalSection(&hLock);
				WaitForSingleObject(hWriterWakeup, INFINITE);

				// the beauty of using Events here is that even if we switch out, and a AcquireRead+ReleaseRead process is done, the event
				// will be marked as re-triggered so WaitForSingleObject will return instantly next time.
				EnterCriticalSection(&hLock);
				continue;
			}

			// got the entire lock exclusively, exit
			break;
		}
	}
	
	void ReleaseWriteLock()
	{
		if( (--_writers) > 0 )
		{
			// still got one or more thread waiting on write
			SetEvent(&hWriterWakeup);
		}

		LeaveCriticalSection(&hLock);
	}
};

#else

// UNIX - this is a simple wrapper
class RWLock
{
	pthread_rwlock_t m_lock;

public:

	RWLock()
	{
		pthread_rwlock_init(&m_lock, NULL);
	}

	~RWLock()
	{
		pthread_rwlock_destroy(&m_lock);
	}

	void AcquireReadLock()
	{
		pthread_rwlock_rdlock(&m_lock);
	}

	void ReleaseReadLock()
	{
		pthread_rwlock_unlock(&m_lock);
	}

	void AcquireWriteLock()
	{
		pthread_rwlock_wrlock(&m_lock);
	}

	void ReleaseWriteLock()
	{
		pthread_rwlock_unlock(&m_lock);
	}
};

#endif//WIN32

#endif//RWLOCK_H
