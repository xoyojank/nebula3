#include "UDPForwarder.h"
#include "GetTime.h"
#include "MTUSize.h"
#include "SocketLayer.h"
#include "WSAStartupSingleton.h"
#include "RakSleep.h"

using namespace RakNet;
static const unsigned short DEFAULT_MAX_FORWARD_ENTRIES=64;

RAK_THREAD_DECLARATION(UpdateUDPForwarder);

bool operator<( const DataStructures::MLKeyRef<UDPForwarder::SrcAndDest> &inputKey, const UDPForwarder::ForwardEntry *cls )
{
	return inputKey.Get().source < cls->srcAndDest.source ||
		(inputKey.Get().source == cls->srcAndDest.source && inputKey.Get().destination < cls->srcAndDest.destination);
}
bool operator>( const DataStructures::MLKeyRef<UDPForwarder::SrcAndDest> &inputKey, const UDPForwarder::ForwardEntry *cls )
{
	return inputKey.Get().source > cls->srcAndDest.source ||
		(inputKey.Get().source == cls->srcAndDest.source && inputKey.Get().destination > cls->srcAndDest.destination);
}
bool operator==( const DataStructures::MLKeyRef<UDPForwarder::SrcAndDest> &inputKey, const UDPForwarder::ForwardEntry *cls )
{
	return inputKey.Get().source == cls->srcAndDest.source && inputKey.Get().destination == cls->srcAndDest.destination;
}


UDPForwarder::ForwardEntry::ForwardEntry() {readSocket=INVALID_SOCKET; timeLastDatagramForwarded=RakNet::GetTime();}
UDPForwarder::ForwardEntry::~ForwardEntry() {if (readSocket!=INVALID_SOCKET) closesocket(readSocket);}

UDPForwarder::UDPForwarder()
{
#ifdef _WIN32
	WSAStartupSingleton::AddRef();
#endif

	maxForwardEntries=DEFAULT_MAX_FORWARD_ENTRIES;
}
UDPForwarder::~UDPForwarder()
{
	Shutdown();

#ifdef _WIN32
	WSAStartupSingleton::Deref();
#endif
}
void UDPForwarder::Startup(void)
{
	isRunning=true;
	threadRunning=false;

#ifdef UDP_FORWARDER_EXECUTE_THREADED
	int errorCode = RakNet::RakThread::Create(UpdateUDPForwarder, this);
	if ( errorCode != 0 )
	{
		RakAssert(0);
		return;
	}

	while (threadRunning==false)
		RakSleep(30);
#endif
}
void UDPForwarder::Shutdown(void)
{
	isRunning=false;

#ifdef UDP_FORWARDER_EXECUTE_THREADED
	while (threadRunning==true)
		RakSleep(30);
#endif

	forwardList.ClearPointers(true,__FILE__,__LINE__);
}
void UDPForwarder::Update(void)
{
#ifndef UDP_FORWARDER_EXECUTE_THREADED
	UpdateThreaded();
#endif

}
void UDPForwarder::UpdateThreaded(void)
{
	fd_set      readFD;
	//fd_set exceptionFD;
	FD_ZERO(&readFD);
//	FD_ZERO(&exceptionFD);
	timeval tv;
	int selectResult;
	tv.tv_sec=0;
	tv.tv_usec=0;

	RakNetTime curTime = RakNet::GetTime();

	SOCKET largestDescriptor=0;
	DataStructures::DefaultIndexType i;

	// Remove unused entries
	i=0;
	while (i < forwardList.GetSize())
	{
		if (curTime > forwardList[i]->timeLastDatagramForwarded && // Account for timestamp wrap
			curTime > forwardList[i]->timeLastDatagramForwarded+forwardList[i]->timeoutOnNoDataMS)
		{
			RakNet::OP_DELETE(forwardList[i],__FILE__,__LINE__);
			forwardList.RemoveAtIndex(i,__FILE__,__LINE__);
		}
		else
			i++;
	}

	if (forwardList.GetSize()==0)
		return;

	for (i=0; i < forwardList.GetSize(); i++)
	{
#ifdef _MSC_VER
#pragma warning( disable : 4127 ) // warning C4127: conditional expression is constant
#endif
		FD_SET(forwardList[i]->readSocket, &readFD);
//		FD_SET(forwardList[i]->readSocket, &exceptionFD);
	}
	largestDescriptor = forwardList[forwardList.GetSize()-1]->readSocket;

#if defined(_PS3) || defined(__PS3__) || defined(SN_TARGET_PS3)
	selectResult=socketselect(largestDescriptor+1, &readFD, 0, 0, &tv);
#else
	selectResult=(int) select((int) largestDescriptor+1, &readFD, 0, 0, &tv);
#endif

	char data[ MAXIMUM_MTU_SIZE ];
	sockaddr_in sa;
	socklen_t len2;

	if (selectResult > 0)
	{
		for (i=0; i < forwardList.GetSize(); i++)
		{
			ForwardEntry *feSource = forwardList[i];
			if (FD_ISSET(feSource->readSocket, &readFD))
			{
				const int flag=0;
				int receivedDataLen, len=0;
				unsigned short portnum=0;
				len2 = sizeof( sa );
				sa.sin_family = AF_INET;
				receivedDataLen = recvfrom( feSource->readSocket, data, MAXIMUM_MTU_SIZE, flag, ( sockaddr* ) & sa, ( socklen_t* ) & len2 );
				portnum = ntohs( sa.sin_port );

				if (feSource->srcAndDest.source.binaryAddress==sa.sin_addr.s_addr)
				{
					if (feSource->updatedSourceAddress==false)
					{
						feSource->updatedSourceAddress=true;

						if (feSource->srcAndDest.source.port!=portnum)
						{
							// Remove both source and dest from list, update addresses, and reinsert in order
							forwardList.RemoveAtIndex(i,__FILE__,__LINE__);
							DataStructures::DefaultIndexType destIndex;
							SrcAndDest srcAndDest;
							srcAndDest.source=feSource->srcAndDest.destination;
							srcAndDest.destination=feSource->srcAndDest.source;
							destIndex=forwardList.GetIndexOf(srcAndDest);
							ForwardEntry *feDest = forwardList[destIndex];
							forwardList.RemoveAtIndex(destIndex,__FILE__,__LINE__);
							feSource->srcAndDest.source.port=portnum;
							feDest->srcAndDest.destination.port=portnum;
							// Reinsert to preserve list order
							if (forwardList.GetIndexOf(feSource->srcAndDest)==(DataStructures::DefaultIndexType)-1)
								forwardList.Push(feSource,feSource->srcAndDest,__FILE__,__LINE__);
							else
								RakNet::OP_DELETE(feSource,__FILE__,__LINE__);
							if (forwardList.GetIndexOf(feDest->srcAndDest)==(DataStructures::DefaultIndexType)-1)
								forwardList.Push(feDest,feDest->srcAndDest,__FILE__,__LINE__);
							else
								RakNet::OP_DELETE(feDest,__FILE__,__LINE__);

							feSource->timeLastDatagramForwarded=curTime;
							feDest->timeLastDatagramForwarded=curTime;
						}
					}

					if (feSource->srcAndDest.source.port==portnum)
					{
						// Forward to destination
						len=0;
						sockaddr_in saOut;
						saOut.sin_port = htons( feSource->srcAndDest.destination.port ); // User port
						saOut.sin_addr.s_addr = feSource->srcAndDest.destination.binaryAddress;
						saOut.sin_family = AF_INET;
						do
						{
							len = sendto( feSource->writeSocket, data, receivedDataLen, 0, ( const sockaddr* ) & saOut, sizeof( saOut ) );
						}
						while ( len == 0 );

						feSource->timeLastDatagramForwarded=curTime;
					}
				}
			}
		}
	}
}
void UDPForwarder::SetMaxForwardEntries(unsigned short maxEntries)
{
	RakAssert(maxEntries>0 && maxEntries<65535/2);
	maxForwardEntries=maxEntries;
}
int UDPForwarder::GetMaxForwardEntries(void) const
{
	return maxForwardEntries;
}
int UDPForwarder::GetUsedForwardEntries(void) const
{
	return (int) forwardList.GetSize();
}
unsigned short UDPForwarder::AddForwardingEntry(SrcAndDest srcAndDest, RakNetTimeMS timeoutOnNoDataMS, const char *forceHostAddress)
{
	DataStructures::DefaultIndexType insertionIndex;
	insertionIndex = forwardList.GetInsertionIndex(srcAndDest);
	if (insertionIndex!=(DataStructures::DefaultIndexType)-1)
	{
		int sock_opt;
		sockaddr_in listenerSocketAddress;
		listenerSocketAddress.sin_port = 0;
		ForwardEntry *fe = RakNet::OP_NEW<UDPForwarder::ForwardEntry>(__FILE__,__LINE__);
		fe->srcAndDest=srcAndDest;
		fe->timeoutOnNoDataMS=timeoutOnNoDataMS;
		fe->readSocket = socket( AF_INET, SOCK_DGRAM, 0 );
		fe->updatedSourceAddress=false;

		// This doubles the max throughput rate
		sock_opt=1024*256;
		setsockopt(fe->readSocket, SOL_SOCKET, SO_RCVBUF, ( char * ) & sock_opt, sizeof ( sock_opt ) );

		// Immediate hard close. Don't linger the readSocket, or recreating the readSocket quickly on Vista fails.
		sock_opt=0;
		setsockopt(fe->readSocket, SOL_SOCKET, SO_LINGER, ( char * ) & sock_opt, sizeof ( sock_opt ) );

		listenerSocketAddress.sin_family = AF_INET;

		if (forceHostAddress && forceHostAddress[0])
		{
			listenerSocketAddress.sin_addr.s_addr = inet_addr( forceHostAddress );
		}
		else
		{
			listenerSocketAddress.sin_addr.s_addr = INADDR_ANY;
		}

		int ret = bind( fe->readSocket, ( struct sockaddr * ) & listenerSocketAddress, sizeof( listenerSocketAddress ) );
		if (ret==-1)
		{
			RakNet::OP_DELETE(fe,__FILE__,__LINE__);
			return 0;
		}

		DataStructures::DefaultIndexType oldSize = forwardList.GetSize();
		forwardList.InsertAtIndex(fe,insertionIndex,__FILE__,__LINE__);
		RakAssert(forwardList.GetIndexOf(fe->srcAndDest)!=-1);
		RakAssert(forwardList.GetSize()==oldSize+1);
		return SocketLayer::GetLocalPort ( fe->readSocket );
	}
	else
	{
		// Takeover existing
		DataStructures::DefaultIndexType existingSrcIndex,existingDstIndex;
		SrcAndDest dest;
		ForwardEntry *feSrc, *feDst;
		dest.destination=srcAndDest.source;
		dest.source=srcAndDest.destination;
		existingSrcIndex = forwardList.GetIndexOf(srcAndDest);
		feSrc=forwardList[existingSrcIndex];
		existingDstIndex = forwardList.GetIndexOf(dest);
		feSrc->timeLastDatagramForwarded=RakNet::GetTime();
		if (existingDstIndex==(DataStructures::DefaultIndexType)-1)
		{
			feDst=0;
		}
		else
		{
			// Refresh destination so communication isn't unidirectional
			feDst=forwardList[existingDstIndex];
			feDst->timeLastDatagramForwarded=feSrc->timeLastDatagramForwarded;
		}


		return SocketLayer::GetLocalPort ( feSrc->readSocket );
	}
}
UDPForwarderResult UDPForwarder::StartForwarding(SystemAddress source, SystemAddress destination, RakNetTimeMS timeoutOnNoDataMS, const char *forceHostAddress,
								   unsigned short *srcToDestPort, unsigned short *destToSourcePort)
{
	// Invalid parameters?
	if (timeoutOnNoDataMS == 0 || timeoutOnNoDataMS > UDP_FORWARDER_MAXIMUM_TIMEOUT || source==UNASSIGNED_SYSTEM_ADDRESS || destination==UNASSIGNED_SYSTEM_ADDRESS)
		return UDPFORWARDER_INVALID_PARAMETERS;

#ifdef UDP_FORWARDER_EXECUTE_THREADED
	ThreadOperation threadOperation;
	threadOperation.source=source;
	threadOperation.destination=destination;
	threadOperation.timeoutOnNoDataMS;
	threadOperation.forceHostAddress=forceHostAddress;
	threadOperation.operation=ThreadOperation::TO_START_FORWARDING;
	threadOperationIncomingMutex.Lock();
	threadOperationIncomingQueue.Push(threadOperation);
	threadOperationIncomingMutex.Unlock();

	while (1)
	{
		RakSleep(0);
		threadOperationOutgoingMutex.Lock();
		if (threadOperationOutgoingQueue.Size()!=0)
		{
			threadOperation=threadOperationOutgoingQueue.Pop();
			threadOperationOutgoingMutex.Unlock();
			*srcToDestPort=threadOperation.srcToDestPort;
			*destToSourcePort=threadOperation.destToSourcePort;
			return threadOperation.result;
		}
		threadOperationOutgoingMutex.Unlock();

	}
#else
	return StartForwardingThreaded(source, destination, timeoutOnNoDataMS, forceHostAddress, srcToDestPort, destToSourcePort);
#endif

}
UDPForwarderResult UDPForwarder::StartForwardingThreaded(SystemAddress source, SystemAddress destination, RakNetTimeMS timeoutOnNoDataMS, const char *forceHostAddress,
								   unsigned short *srcToDestPort, unsigned short *destToSourcePort)
{
	SrcAndDest srcAndDest;
	srcAndDest.destination=destination;
	srcAndDest.source=source;
	SrcAndDest destAndSrc;
	destAndSrc.destination=source;
	destAndSrc.source=destination;

// Just takeover existing if this happens
//	if (forwardList.GetIndexOf(srcAndDest) != (DataStructures::DefaultIndexType) -1)
//		return UDPFORWARDER_FORWARDING_ALREADY_EXISTS;

	*srcToDestPort = AddForwardingEntry(srcAndDest, timeoutOnNoDataMS, forceHostAddress);

	if (*srcToDestPort==0)
		return UDPFORWARDER_NO_SOCKETS;

	*destToSourcePort = AddForwardingEntry(destAndSrc, timeoutOnNoDataMS, forceHostAddress);

	if (*destToSourcePort==0)
		return UDPFORWARDER_NO_SOCKETS;

	DataStructures::DefaultIndexType idxSrcAndDest, idxDestAndSrc;
	idxSrcAndDest = forwardList.GetIndexOf(srcAndDest);
	idxDestAndSrc = forwardList.GetIndexOf(destAndSrc);
	forwardList[idxSrcAndDest]->writeSocket=forwardList[idxDestAndSrc]->readSocket;
	forwardList[idxDestAndSrc]->writeSocket=forwardList[idxSrcAndDest]->readSocket;

	return UDPFORWARDER_SUCCESS;
}
void UDPForwarder::StopForwarding(SystemAddress source, SystemAddress destination)
{
#ifdef UDP_FORWARDER_EXECUTE_THREADED
	ThreadOperation threadOperation;
	threadOperation.source=source;
	threadOperation.destination=destination;
	threadOperation.operation=ThreadOperation::TO_STOP_FORWARDING;
	threadOperationIncomingMutex.Lock();
	threadOperationIncomingQueue.Push(threadOperation);
	threadOperationIncomingMutex.Unlock();
#else
	StopForwardingThreaded(source, destination);
#endif
}
void UDPForwarder::StopForwardingThreaded(SystemAddress source, SystemAddress destination)
{

	SrcAndDest srcAndDest;
	srcAndDest.destination=destination;
	srcAndDest.source=source;
	DataStructures::DefaultIndexType idx = forwardList.GetIndexOf(srcAndDest);
	if (idx!=(DataStructures::DefaultIndexType)-1)
	{
		RakNet::OP_DELETE(forwardList[idx],__FILE__,__LINE__);
		forwardList.RemoveAtIndex(idx,__FILE__,__LINE__);
	}

	srcAndDest.destination=source;
	srcAndDest.source=destination;

	idx = forwardList.GetIndexOf(srcAndDest);
	if (idx!=(DataStructures::DefaultIndexType)-1)
	{
		RakNet::OP_DELETE(forwardList[idx],__FILE__,__LINE__);
		forwardList.RemoveAtIndex(idx,__FILE__,__LINE__);
	}
}
#ifdef UDP_FORWARDER_EXECUTE_THREADED
RAK_THREAD_DECLARATION(UpdateUDPForwarder)
{
	UDPForwarder * udpForwarder = ( UDPForwarder * ) arguments;
	udpForwarder->threadRunning=true;
	UDPForwarder::ThreadOperation threadOperation;
	while (udpForwarder->isRunning)
	{
		udpForwarder->threadOperationIncomingMutex.Lock();
		while (udpForwarder->threadOperationIncomingQueue.Size())
		{
			threadOperation=udpForwarder->threadOperationIncomingQueue.Pop();
			udpForwarder->threadOperationIncomingMutex.Unlock();
			if (threadOperation.operation==UDPForwarder::ThreadOperation::TO_START_FORWARDING)
			{
				threadOperation.result=udpForwarder->StartForwardingThreaded(threadOperation.source, threadOperation.destination, threadOperation.timeoutOnNoDataMS,
					threadOperation.forceHostAddress, &threadOperation.srcToDestPort, &threadOperation.destToSourcePort);
				udpForwarder->threadOperationOutgoingMutex.Lock();
				udpForwarder->threadOperationOutgoingQueue.Push(threadOperation);
				udpForwarder->threadOperationOutgoingMutex.Unlock();
			}
			else
			{
				udpForwarder->StopForwardingThreaded(threadOperation.source, threadOperation.destination);
			}


			udpForwarder->threadOperationIncomingMutex.Lock();
		}
		udpForwarder->threadOperationIncomingMutex.Unlock();

		udpForwarder->UpdateThreaded();
		RakSleep(0);
	}
	udpForwarder->threadRunning=false;
	return 0;
}
#endif
