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

#ifndef _THREADING_MUTEX_H
#define _THREADING_MUTEX_H
#ifdef __DragonFly__
#  include <pthread.h>
#endif//__DragonFly__

/*#ifdef WIN32
	#ifdef _DEBUG
		#define ASCENT_INLINE 
		#define ASCENT_FORCEINLINE __forceinline
	#else
		#define ASCENT_INLINE __forceinline
		#define ASCENT_FORCEINLINE __forceinline
	#endif
#else
	#define ASCENT_INLINE inline
	#define ASCENT_FORCEINLINE inline
#endif

#ifdef WIN32
	#ifndef SCRIPTLIB
		#define SERVER_DECL __declspec(dllexport)
		#define SCRIPT_DECL __declspec(dllimport)
	#else
		#define SERVER_DECL __declspec(dllimport)
		#define SCRIPT_DECL __declspec(dllexport)
	#endif
#else
	#define SERVER_DECL 
	#define SCRIPT_DECL 
#endif*/
             
class SERVER_DECL Mutex
{
public:
	friend class Condition;

	/** Initializes a mutex class, with InitializeCriticalSection / pthread_mutex_init
	 */
	Mutex();

	/** Deletes the associated critical section / mutex
	 */
	~Mutex();

	/** Acquires this mutex. If it cannot be acquired immediately, it will block.
	 */
	ASCENT_INLINE void Acquire()
	{
#ifndef WIN32
		pthread_mutex_lock(&mutex);
#else
		EnterCriticalSection(&cs);
#endif//WIN32
	}

	/** Releases this mutex. No error checking performed
	 */
	ASCENT_INLINE void Release()
	{
#ifndef WIN32
		pthread_mutex_unlock(&mutex);
#else
		LeaveCriticalSection(&cs);
#endif//WIN32
	}

	/** Attempts to acquire this mutex. If it cannot be acquired (held by another thread)
	 * it will return false.
	 * @return false if cannot be acquired, true if it was acquired.
	 */
	ASCENT_INLINE bool AttemptAcquire()
	{
#ifndef WIN32
		return (pthread_mutex_trylock(&mutex) == 0);
#else
		return (TryEnterCriticalSection(&cs) == TRUE ? true : false);
#endif//WIN32
	}

	void lock()
	{
		Acquire();
	}

	void unlock()
	{
		Release();
	}

	bool trylock()
	{
		return AttemptAcquire();
	}

protected:
#ifdef WIN32
	/** Critical section used for system calls
	 */
	CRITICAL_SECTION cs;

#else
	/** Static mutex attribute
	 */
	static bool attr_initalized;
	static pthread_mutexattr_t attr;

	/** pthread struct used in system calls
	 */
	pthread_mutex_t mutex;
#endif//WIN32
};

class LockMutex {
public:
	LockMutex(Mutex* in_mut, bool iLock = true);
	~LockMutex();
	void unlock();
	void lock();
private:
	bool	locked;
	Mutex*	mut;
};


#ifdef WIN32

class SERVER_DECL FastMutex
{
#pragma pack(push,8)
	volatile long m_lock;
#pragma pack(pop)
	DWORD m_recursiveCount;

public:
	ASCENT_INLINE FastMutex() : m_lock(0),m_recursiveCount(0) {}
	ASCENT_INLINE ~FastMutex() {}

	ASCENT_INLINE void Acquire()
	{
		DWORD thread_id = GetCurrentThreadId(), owner;
		if(thread_id == (DWORD)m_lock)
		{
			++m_recursiveCount;
			return;
		}

		for(;;)
		{
			owner = InterlockedCompareExchange(&m_lock, thread_id, 0);
			if(owner == 0)
				break;

			Sleep(0);
		}

		++m_recursiveCount;
	}

	ASCENT_INLINE bool AttemptAcquire()
	{
		DWORD thread_id = GetCurrentThreadId();
		if(thread_id == (DWORD)m_lock)
		{
			++m_recursiveCount;
			return true;
		}

		DWORD owner = InterlockedCompareExchange(&m_lock, thread_id, 0);
		if(owner == 0)
		{
			++m_recursiveCount;
			return true;
		}

		return false;
	}

	ASCENT_INLINE void Release()
	{
		if((--m_recursiveCount) == 0)
			InterlockedExchange(&m_lock, 0);
	}
};

#else

#define FastMutex Mutex

#endif//WIN32

#endif//_THREADING_MUTEX_H
