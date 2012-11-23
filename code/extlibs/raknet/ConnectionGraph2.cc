#include "ConnectionGraph2.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"

void ConnectionGraph2::OnClosedConnection(SystemAddress systemAddress, RakNetGUID rakNetGUID, PI2_LostConnectionReason lostConnectionReason )
{
	// Send notice to all existing connections
	RakNet::BitStream bs;
	if (lostConnectionReason==LCR_CONNECTION_LOST)
		bs.Write((MessageID)ID_REMOTE_CONNECTION_LOST);
	else
		bs.Write((MessageID)ID_REMOTE_DISCONNECTION_NOTIFICATION);
	bs.Write(systemAddress);
	bs.Write(rakNetGUID);
	rakPeerInterface->Send(&bs,HIGH_PRIORITY,RELIABLE_ORDERED,0,systemAddress,true);
}
void ConnectionGraph2::OnNewConnection(SystemAddress systemAddress, RakNetGUID rakNetGUID, bool isIncoming)
{
	(void) isIncoming;

	// Send all existing systems to new connection
	RakNet::BitStream bs;
	bs.Write((MessageID)ID_REMOTE_NEW_INCOMING_CONNECTION);
	bs.Write((unsigned int)1);
	bs.Write(systemAddress);
	bs.Write(rakNetGUID);
	rakPeerInterface->Send(&bs,HIGH_PRIORITY,RELIABLE_ORDERED,0,systemAddress,true);

	// Send everyone to the new guy
	DataStructures::List<SystemAddress> addresses;
	DataStructures::List<RakNetGUID> guids;
	rakPeerInterface->GetSystemList(addresses, guids);
	bs.Reset();
	bs.Write((MessageID)ID_REMOTE_NEW_INCOMING_CONNECTION);
	BitSize_t writeOffset = bs.GetWriteOffset();
	bs.Write(addresses.Size());

	unsigned int i;
	unsigned int count=0;
	for (i=0; i < addresses.Size(); i++)
	{
		if (addresses[i]==systemAddress)
			continue;
		
		bs.Write(addresses[i]);
		bs.Write(guids[i]);
		count++;
	}

	if (count>0)
	{
		BitSize_t writeOffset2 = bs.GetWriteOffset();
		bs.SetWriteOffset(writeOffset);
		bs.Write(count);
		bs.SetWriteOffset(writeOffset2);
		rakPeerInterface->Send(&bs,HIGH_PRIORITY,RELIABLE_ORDERED,0,systemAddress,false);
	}
}
