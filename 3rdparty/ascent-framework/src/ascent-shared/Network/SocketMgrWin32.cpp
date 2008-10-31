/*
 * Multiplatform Async Network Library
 * Copyright (c) 2007 Burlex
 *
 * SocketMgr - iocp-based SocketMgr for windows.
 *
 */

#include "Network/Network.h"
#ifdef CONFIG_USE_IOCP

#include "Config/ConfigEnv.h"
#include "CrashHandler.h"

initialiseSingleton(SocketMgr);
SocketMgr::SocketMgr()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,0), &wsaData);
	m_completionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, (ULONG_PTR)0, 0);
}

SocketMgr::~SocketMgr()
{

}

void SocketMgr::SpawnWorkerThreads()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	threadcount = si.dwNumberOfProcessors*2;
	//int tc = si.dwNumberOfProcessors;
	//if( Config.MainConfig.GetInt("Network", "WorkerThreadCount", &tc) )
	//	threadcount = tc;
	//else
	//	threadcount = si.dwNumberOfProcessors;

	printf("IOCP: Spawning %u worker threads.\n", threadcount);
	for(long x = 0; x < threadcount; ++x)
	{
		SocketWorkerThread* workerThread = new SocketWorkerThread();
		SetThreadName("Ascent IOCP workerthread:%d", x);
		ThreadPool.ExecuteTask(workerThread);		
	}
}

void SocketMgr::OnRehash()
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	threadcount = si.dwNumberOfProcessors*2;
	int tc;
	int i, diff;
	//if( !Config.MainConfig.GetInt("Network", "WorkerThreadCount", &tc) )
		tc = si.dwNumberOfProcessors;

	if( threadcount != tc )
	{
		// need to change threads
		if( tc > threadcount )
		{
			// spawn more
			diff = tc - threadcount;
			printf("IOCP::Rehash Spawning %u more worker threads\n", diff);
			for(i = 0; i < diff; ++i)
				ThreadPool.ExecuteTask(new SocketWorkerThread());
		}
		else
		{
			// kill some
			diff = threadcount - tc;
			printf("IOCP::Rehash Killing %u worker threads\n", diff);
			for(i = 0; i < diff; ++i)
			{
				OverlappedStruct * ov = new OverlappedStruct(SOCKET_IO_THREAD_SHUTDOWN);
				PostQueuedCompletionStatus(m_completionPort, 0, (ULONG_PTR)0, &ov->m_overlap);
			}
		}

		threadcount = tc;
		printf("IOCP Thread count is now %u\n", threadcount);
	}
}

#ifndef _WIN64
#  define IOCP_SOCKET_PTR LPDWORD
#else
#  define IOCP_SOCKET_PTR PULONG_PTR
#endif//_WIN64

bool SocketWorkerThread::run()
{
	HANDLE cp = sSocketMgr.GetCompletionPort();
	DWORD len;
	Socket * s;
	OverlappedStruct * ov;
	LPOVERLAPPED ol_ptr;

	while(true)
	{
		if(!GetQueuedCompletionStatus(cp, &len, (IOCP_SOCKET_PTR)&s, &ol_ptr, 10000))
			continue;

		ov = CONTAINING_RECORD(ol_ptr, OverlappedStruct, m_overlap);

		if(ov->m_event == SOCKET_IO_THREAD_SHUTDOWN)
		{
			SafeDelete(ov);
			return true;
		}

		if(ov->m_event < NUM_SOCKET_IO_EVENTS)
			ophandlers[ov->m_event](s, len);
	}

	return true;
}

void HandleReadComplete(Socket * s, uint32 len)
{
	//s->m_readEvent=NULL;
	if(!s->IsDeleted())
	{
		s->m_readEvent.Unmark();
		if(len > 0)
		{
			s->GetReadBuffer().IncrementWritten(len);
			s->OnRead();
			s->SetupReadEvent();
		}
		else
			s->Delete();	  // Queue deletion.
	}
}

void HandleWriteComplete(Socket * s, uint32 len)
{
	if(!s->IsDeleted())
	{
		s->m_writeEvent.Unmark();
		s->BurstBegin();					// Lock
		s->GetWriteBuffer().Remove(len);
		if( s->GetWriteBuffer().GetContiguiousBytes() > 0 )
			s->WriteCallback();
		else
			s->DecSendLock();
		s->BurstEnd();					  // Unlock
	}
}

void HandleShutdown(Socket * /*s*/, uint32 /*len*/)
{

}

void SocketMgr::CloseAll()
{
	list<Socket*> tokill;

	socketLock.Acquire();
	for(set<Socket*>::iterator itr = _sockets.begin(); itr != _sockets.end(); ++itr)
		tokill.push_back(*itr);
	socketLock.Release();

	for(list<Socket*>::iterator itr = tokill.begin(); itr != tokill.end(); ++itr)
		(*itr)->Disconnect();

	size_t size;
	do
	{
		socketLock.Acquire();
		size = _sockets.size();
		socketLock.Release();
	}while(size);
}

void SocketMgr::ShutdownThreads()
{
	for(int i = 0; i < threadcount; ++i)
	{
		OverlappedStruct * ov = new OverlappedStruct(SOCKET_IO_THREAD_SHUTDOWN);
		PostQueuedCompletionStatus(m_completionPort, 0, (ULONG_PTR)0, &ov->m_overlap);
	}
}

#endif//CONFIG_USE_IOCP
