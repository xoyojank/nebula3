/// \file
/// \brief Contains the third iteration of the ReplicaManager class.
///
/// This file is part of RakNet Copyright 2003 Jenkins Software LLC
///
/// Usage of RakNet is subject to the appropriate license agreement.

#ifndef __REPLICA_MANAGER_3
#define __REPLICA_MANAGER_3

#include "DS_Multilist.h"
#include "RakNetTypes.h"
#include "RakNetTime.h"
#include "BitStream.h"
#include "PacketPriority.h"
#include "PluginInterface2.h"
#include "NetworkIDObject.h"
#include "RakNetSmartPtr.h"

namespace RakNet
{
class Connection_RM3;
class Replica3;


/// \internal
struct PRO
{
	/// Passed to RakPeerInterface::Send(). Defaults to ReplicaManager3::SetDefaultPacketPriority().
	PacketPriority priority;

	/// Passed to RakPeerInterface::Send(). Defaults to ReplicaManager3::SetDefaultPacketReliability().
	PacketReliability reliability;

	/// Passed to RakPeerInterface::Send(). Defaults to ReplicaManager3::SetDefaultOrderingChannel().
	char orderingChannel;
};

/// \brief System to help automate game object construction, destruction, and serialization
/// ReplicaManager3 tracks your game objects and automates the networking for replicating them across the network
/// As objects are created, destroyed, or serialized differently, those changes are pushed out to other systems
/// To use:
/// 1. Derive from Connection_RM3 and implement Connection_RM3::AllocReplica(). This is a factory function where given an identifier for a class (such as name) return an instance of that class. Should be able to return any networked object in your game.
/// 2. Derive from ReplicaManager3 and implement AllocConnection() and DeallocConnection() to return the class you created in step 1.
/// 3. Derive your networked game objects from Replica3. All pure virtuals have to be implemented, however defaults are provided for Replica3::QueryConstruction(), Replica3::QueryRemoteConstruction(), and Replica3::QuerySerialization() depending on your network architecture.
/// 4. When a new game object is created on the local system, pass it to ReplicaManager3::Reference().
/// 5. When a game object is destroyed on the local system, and you want other systems to know about it, call Replica3::BroadcastDestruction()
///
/// At this point, all new connections will automatically download, get construction messages, get destruction messages, and update serialization automatically.
class RAK_DLL_EXPORT ReplicaManager3 : public PluginInterface2
{
public:
	ReplicaManager3();
	virtual ~ReplicaManager3();

	/// \brief Implement to return a game specific derivation of Connection_RM3
	/// The connection object represents a remote system connected to you that is using the ReplicaManager3 system
	/// It has functions to perform operations per-connection
	/// AllocConnection() and DeallocConnection() are factory functions to create and destroy instances of the connection object
	/// It is used if connections are automanaged (which they are by default).
	/// \sa SetAutoManageConnections()
	/// \param[in] systemAddress Address of the system you are adding
	/// \param[in] rakNetGUID GUID of the system you are adding. See Packet::rakNetGUID or RakPeerInterface::GetGUIDFromSystemAddress()
	/// \return The new connection instance.
	virtual Connection_RM3* AllocConnection(SystemAddress systemAddress, RakNetGUID rakNetGUID) const=0;

	/// \brief Implement to destroy a class instanced returned by AllocConnection()
	/// Most likely just implement as {delete connection;}
	/// \param[in] connection The pointer instance to delete
	virtual void DeallocConnection(Connection_RM3 *connection) const=0;

	/// \brief Enable or disable automatically assigning connections to new instances of Connection_RM3
	/// ReplicaManager3 can automatically create and/or destroy Connection_RM3 as systems connect or disconnect from RakPeerInterface
	/// By default this is on, to make the system faster to setup
	/// If you don't want all connections to take part in the game, or you want to delay when a connection downloads the game, set \a autoCreate to false
	/// If you want to delay deleting a connection that has dropped, set \a autoDestroy to false. If you do this, then you must call PopConnection() to remove that connection from being internally tracked. You'll also have to delete the connection instance on your own.
	/// \param[in] autoCreate Automatically call ReplicaManager3::AllocConnection() for each new connection. Defaults to true.
	/// \param[in] autoDestroy Automatically call ReplicaManager3::DeallocConnection() for each dropped connection. Defaults to true.
	void SetAutoManageConnections(bool autoCreate, bool autoDestroy);

	/// \brief Track a new Connection_RM3 instance
	/// If \a autoCreate is false for SetAutoManageConnections(), then you need to add new instances of Connection_RM3 yourself
	/// Use PushConnection() to do so
	/// You don't need to track this pointer yourself, you can get it with GetConnectionAtIndex(), GetConnectionByGUID(), or GetConnectionBySystemAddress()
	/// \param[in] newConnection The new connection instance to track.
	bool PushConnection(RakNet::Connection_RM3 *newConnection);

	/// \brief Stop tracking a connection, and optionally destroy all objects created by that system
	/// If \a autoDestroy is false for SetAutoManageConnections(), then you need to manually have the system stop connections.
	/// PopConnection() will do so, and return that connection instance to you.
	/// \param[in] guid of the connection to get. Passed to ReplicaManager3::AllocConnection() originally. 
	RakNet::Connection_RM3 * PopConnection(RakNetGUID guid);

	/// \brief Adds a replicated object to the system.
	/// Anytime you create a new object that derives from Replica3, and you want ReplicaManager3 to use it, pass it to Reference()
	/// Remote systems already connected will download this object the next time ReplicaManager3::Update() is called, which happens every time you call RakPeerInterface::Receive()
	/// \param[in] replica3 The object to start tracking
	void Reference(RakNet::Replica3 *replica3);

	/// \brief Removes a replicated object from the system.
	/// The object is not deallocated, it is up to the caller to do so.
	/// This is called automatically from the destructor of Replica3, so you don't need to call it manually unless you want to stop tracking an object before it is destroyed.
	/// \param[in] replica3 The object to stop tracking
	void Dereference(RakNet::Replica3 *replica3);

	/// \brief Removes multiple replicated objects from the system.
	/// Same as Dereference(), but for a list of objects.
	/// Useful if also using GetReplicasCreatedByGuid(), GetReplicasCreatedByMe(), or  GetReferencedReplicaList()
	/// \param[in] replicaListIn List of objects
	void DereferenceList(DataStructures::Multilist<ML_STACK, Replica3*> &replicaListIn);

	/// \brief Returns all objects originally created by a particular system
	/// Originally created is defined as the value of Replica3::creatingSystemGUID, which is automatically assigned in ReplicaManager3::Reference()
	/// You do not have to be directly connected to that system to get the objects originally created by that system
	/// \param[in] guid GUID of the system we are referring to. Originally passed as the \a guid parameter to ReplicaManager3::AllocConnection()
	/// \param[out] List of Replica3 instances to be returned
	void GetReplicasCreatedByGuid(RakNetGUID guid, DataStructures::Multilist<ML_STACK, Replica3*> &replicaListOut);

	/// \brief Returns all objects originally created by your system
	/// Calls GetReplicasCreatedByGuid() for your own system guid.
	/// \param[out] List of Replica3 instances to be returned
	void GetReplicasCreatedByMe(DataStructures::Multilist<ML_STACK, Replica3*> &replicaListOut);

	/// \brief Returns the entire list of Replicas that we know about.
	/// This is all Replica3 instances passed to Reference, as well as instances we downloaded and created via Connection_RM3::AllocReference()
	/// \param[out] List of Replica3 instances to be returned
	void GetReferencedReplicaList(DataStructures::Multilist<ML_STACK, Replica3*> &replicaListOut);

	/// \brief Returns the number of replicas known about
	/// Returns the size of the list that would be returned by GetReferencedReplicaList()
	/// \return How many replica objects are in the list of replica objects
	unsigned GetReplicaCount(void) const;

	/// \brief Returns a replica by index
	/// Returns one of the items in the list that would be returned by GetReferencedReplicaList()
	/// \param[in] index An index, from 0 to GetReplicaCount()-1.
	/// \return A Replica3 instance
	Replica3 *GetReplicaAtIndex(unsigned index);

	/// \brief Returns the number of connections
	/// Returns the number of connections added with ReplicaManager3::PushConnection(), minus the number removed with ReplicaManager3::PopConnection()
	/// \return The number of registered connections
	DataStructures::DefaultIndexType GetConnectionCount(void) const;

	/// Returns a connection pointer previously added with PushConnection()
	/// \param[in] index An index, from 0 to GetConnectionCount()-1.
	/// \return A Connection_RM3 pointer
	Connection_RM3* GetConnectionAtIndex(unsigned index) const;

	/// Returns a connection pointer previously added with PushConnection()
	/// \param[in] sa The system address of the connection to return
	/// \return A Connection_RM3 pointer, or 0 if not found
	Connection_RM3* GetConnectionBySystemAddress(SystemAddress sa) const;

	/// Returns a connection pointer previously added with PushConnection.()
	/// \param[in] guid The guid of the connection to return
	/// \return A Connection_RM3 pointer, or 0 if not found
	Connection_RM3* GetConnectionByGUID(RakNetGUID guid) const;

	/// \param[in] Default ordering channel to use for object creation, destruction, and serializations
	void SetDefaultOrderingChannel(char def);

	/// \param[in] Default packet priority to use for object creation, destruction, and serializations
	void SetDefaultPacketPriority(PacketPriority def);

	/// \param[in] Default packet reliability to use for object creation, destruction, and serializations
	void SetDefaultPacketReliability(PacketReliability def);

	/// \brief Enables or disables the automatic object creation and destruction feature.
	/// If true, then each ReplicaManager3::Update(), Replica3::QueryConstruction() will be called for each locally created instance of Replica3, for each instance of Connection_RM3
	/// Based on the return value of QueryConstruction(), the object will be created or destroyed on that remote system.
	/// Defaults to true
	/// This is not efficient for large worlds where higher level culling mechanisms exist. You can narrow the scope of which objects are checked by using Connection_RM3::SetConstructionByList.
	/// \note Connection_RM3::SetConstructionByList and ReplicaManager3::SetAutoConstructByQuery are mutually exclusive. Do not use both at the same time.
	/// \param[in] autoConstruct True to enable, false to disable.
	void SetAutoConstructByQuery(bool autoConstruct);

	/// Every \a intervalMS milliseconds, Connection_RM3::OnAutoserializeInterval() will be called
	/// Defaults to 30
	/// Pass with 0 to disable
	/// If you want to control the update interval with more granularity, use the return values from Replica3::Serialize()
	/// \param[in] intervalMS How frequently to autoserialize all objects. This controls the maximum number of game object updates per second.
	void SetAutoSerializeInterval(RakNetTime intervalMS);

	/// \brief Return the connections that we think have an instance of the specified Replica3 instance
	/// This can be wrong, for example if that system locally deleted the outside the scope of ReplicaManager3, if QueryRemoteConstruction() returned false, or if DeserializeConstruction() returned false.
	/// \param[in] replica The replica to check against.
	/// \param[out] connectionsThatHaveConstructedThisReplica Populated with connection instances that we believe have \a replica allocated
	void GetConnectionsThatHaveReplicaConstructed(Replica3 *replica, DataStructures::Multilist<ML_STACK, Connection_RM3*> &connectionsThatHaveConstructedThisReplica);

	/// \brief Defines the unique instance of ReplicaManager3 if multiple instances are on the same instance of RakPeerInterface
	/// ReplicaManager3 supports multiple instances of itself attached to the same instance of rakPeer, in case your game has multiple worlds
	/// Call SetWorldID with a different number for each instance
	/// The default worldID is 0
	/// To use multiple worlds, you will also need to call ReplicaManager3::SetNetworkIDManager() to have a different NetworkIDManager instance per world
	void SetWorldID(unsigned char id);

	/// \return Whatever was passed to SetWorldID(), or 0 if it was never called.
	unsigned char GetWorldID(void) const;

	/// Sets the networkIDManager instance that this plugin relys upon.
	/// Uses whatever instance is attached to RakPeerInterface if unset.
	/// To support multiple worlds, you should set it to a different manager for each instance of the plugin
	/// \param[in] _networkIDManager The externally allocated NetworkIDManager instance for this plugin to use.
	void SetNetworkIDManager(NetworkIDManager *_networkIDManager);

	/// Returns what was passed to SetNetworkIDManager(), or the instance on RakPeerInterface if unset.
	NetworkIDManager *GetNetworkIDManager(void) const;

	/// Send a network command to destroy one or more Replica3 instances
	/// Usually you won't need this, but use Replica3::BroadcastDestruction() instead.
	/// The objects are unaffected locally
	/// \param[in] replicaList List of Replica3 objects to tell other systems to destroy.
	/// \param[in] exclusionAddress Which system to not send to. UNASSIGNED_SYSTEM_ADDRESS to send to all.
	void BroadcastDestructionList(DataStructures::Multilist<ML_STACK, Replica3*> &replicaList, SystemAddress exclusionAddress);

	/// \internal
	/// Tell other systems that have this replica to destroy this replica
	/// You shouldn't need to call this, as it happens in the Replica3 destructor
	void BroadcastDestruction(Replica3 *replica, SystemAddress exclusionAddress);

	/// \internal	
	/// Frees internal lists
	/// Externally allocated pointers are not deallocated
	void Clear(void);

	/// \internal
	PRO GetDefaultSendParameters(void) const;

protected:
	virtual PluginReceiveResult OnReceive(Packet *packet);
	virtual void Update(void);
	virtual void OnClosedConnection(SystemAddress systemAddress, RakNetGUID rakNetGUID, PI2_LostConnectionReason lostConnectionReason );
	virtual void OnNewConnection(SystemAddress systemAddress, RakNetGUID rakNetGUID, bool isIncoming);

	void OnConstructionExisting(unsigned char *packetData, int packetDataLength, RakNetGUID senderGuid, unsigned char packetDataOffset);
	void OnConstruction(unsigned char *packetData, int packetDataLength, RakNetGUID senderGuid, unsigned char packetDataOffset);
	void OnSerialize(unsigned char *packetData, int packetDataLength, RakNetGUID senderGuid, RakNetTime timestamp, unsigned char packetDataOffset);
	void OnLocalConstructionRejected(unsigned char *packetData, int packetDataLength, RakNetGUID senderGuid, unsigned char packetDataOffset);
	void OnLocalConstructionAccepted(unsigned char *packetData, int packetDataLength, RakNetGUID senderGuid, unsigned char packetDataOffset);

	Replica3* GetReplicaByNetworkID(NetworkID networkId);

	DataStructures::Multilist<ML_STACK, Connection_RM3*> connectionList;
	DataStructures::Multilist<ML_STACK, Replica3*> userReplicaList;

	PRO defaultSendParameters;
	bool autoConstructByQuery;
	RakNetTime autoSerializeInterval;
	RakNetTime lastAutoSerializeOccurance;
	unsigned char worldId;
	NetworkIDManager *networkIDManager;
	bool autoCreateConnections, autoDestroyConnections;
};

/// Represents the serialized data for an object the last time it was sent. Used by Connection_RM3::OnAutoserializeInterval() and Connection_RM3::SendSerializeIfChanged()
struct LastSerializationResult
{
	LastSerializationResult();
	~LastSerializationResult();

	/// The replica instance we serialized
	RakNet::Replica3 *replica;
	// Last data we sent
	RakNet::BitStream bitStream;
	bool neverSerialize;
};

/// Parameters passed to Replica3::Serialize()
struct SerializeParameters
{
	/// Write your output for serialization here
	/// If nothing is written, the serialization will not occur
	RakNet::BitStream outputBitstream;

	/// What we sent, the last time we sent data over the network
	const RakNet::BitStream *lastSerializationSent;

	/// Set to non-zero to transmit a timestamp with this message.
	/// Defaults to 0
	RakNetTime messageTimestamp;

	/// Passed to RakPeerInterface::Send(). Defaults to ReplicaManager3::SetDefaultPacketPriority().
	/// Passed to RakPeerInterface::Send(). Defaults to ReplicaManager3::SetDefaultPacketReliability().
	/// Passed to RakPeerInterface::Send(). Defaults to ReplicaManager3::SetDefaultOrderingChannel().
	PRO pro;

	/// Passed to RakPeerInterface::Send().
	RakNet::Connection_RM3 *destinationConnection;

	/// For prior serializations this tick, for the same connection, how many bits have we written so far?
	/// Use this to limit how many objects you send to update per-tick if desired
	BitSize_t bitsWrittenSoFar;
};

/// \brief Each remote system is represented by Connection_RM3. Used to allocate Replica3 and track which instances have been allocated
/// Important function: AllocReplica() - must be overridden to create an object given an identifier for that object, which you define for all objects in your game
class RAK_DLL_EXPORT Connection_RM3
{
public:
	Connection_RM3(SystemAddress _systemAddress, RakNetGUID _guid);
	virtual ~Connection_RM3();

	/// \brief Class factory to create a Replica3 instance, given a user-defined identifier
	/// Identifier is returned by Replica3::WriteAllocationID() for what type of class to create
	/// See Replica3::Dealloc for the corresponding destruction message
	/// \sa Replica3::WriteAllocationID()
	/// Return 0 if unable to create the intended object.
	/// Sample implementation:
	/// {RakNet::RakString typeName; allocationIdBitstream->Read(typeName); if (typeName=="Soldier") return new Soldier; return 0;}
	/// \param[in] allocationIdBitstream user-defined bitstream uniquely identifying a game object type
	virtual Replica3 *AllocReplica(RakNet::BitStream *allocationIdBitstream)=0;

	/// \brief Get list of all replicas that are constructed for this connection
	/// \param[out] objectsTheyDoHave Destination list. Returned in sorted ascending order, sorted on the value of the Replica3 pointer.
	virtual void GetConstructedReplicas(DataStructures::Multilist<ML_STACK, Replica3*> &objectsTheyDoHave);

	/// Returns true if we think this remote connection has this replica constructed
	/// \param[in] replica3 Which replica we are querying
	/// \return True if constructed, false othewise
	bool HasReplicaConstructed(RakNet::Replica3 *replica);

	/// Sends over a serialization update for \a replica
	/// NetworkID::GetNetworkID() is written automatically, serializationData is the object data
	/// This is used internally - however, you can also call it manually to send a data update for a remote replica
	/// \param[in] replica Which replica to serialize
	/// \param[in] serializationData Serialized object data
	/// \param[in] timestamp 0 means no timestamp. Otherwise message is prepended with ID_TIMESTAMP
	/// \param[in] sendParameters Parameters on how to send
	/// \param[in] rakPeer Instance of RakPeerInterface to send on
	/// \param[in] worldId Which world, see ReplicaManager3::SetWorldID()
	virtual void SendSerialize(RakNet::Replica3 *replica, RakNet::BitStream *serializationData, RakNetTime timestamp, PRO sendParameters, RakPeerInterface *rakPeer, unsigned char worldId);

	/// \internal
	/// Calls Connection_RM3::SendSerialize() if Replica3::Serialize() returns a different result than what is contained in \a lastSerializationResult
	/// Used by autoserialization in Connection_RM3::OnAutoserializeInterval()
	/// \param[in] constructedReplicasIndex Index into constructedReplicas for whichever replica this is
	/// \param[in] sp Controlling parameters over the serialization
	/// \param[in] rakPeer Instance of RakPeerInterface to send on
	/// \param[in] worldId Which world, see ReplicaManager3::SetWorldID()
	/// \return true if anything was sent, false otherwise
	virtual bool SendSerializeIfChanged(DataStructures::DefaultIndexType constructedReplicasIndex, SerializeParameters *sp, RakPeerInterface *rakPeer, unsigned char worldId);

	/// \internal
	/// Called for each connection when ReplicaManager3::Reference() is called
	/// Adds a Replica3* from the constructedReplicas list
	/// \param[in] replica Which replica to reference
	RakNetSmartPtr<LastSerializationResult> Reference(Replica3* replica);

	/// \internal
	/// Called for each connection when ReplicaManager3::Dereference() is called
	/// Removes a Replica3* from the constructedReplicas list
	/// \param[in] replica Which replica to dereference
	void Dereference(Replica3* replica);

	/// \return The system address passed to the constructor of this object
	SystemAddress GetSystemAddress(void) const {return systemAddress;}

	/// \return Returns the RakNetGUID passed to the constructor of this object
	RakNetGUID GetRakNetGUID(void) const {return guid;}

	/// \brief Destroy objects that no longer exist. Create objects that now exist. objectsThatExist is the entire list of objects that should exist.
	/// Given a list of objects, compare it against constructedReplicas.
	/// Objects in objectsThatExist, that are not in constructedReplicas, and that pass Replica3::QueryConstruction() be constructed.
	/// Objects not in objectsThatExist, that are in constructedReplicas, and that pass Replica3::SerializeDestruction() be destroyed.
	/// This can be more efficient than ReplicaManager3::SetAutoConstructByQuery.
	/// \note Connection_RM3::SetConstructionByList and ReplicaManager3::SetAutoConstructByQuery are mutually exclusive. Do not use both at the same time.
	/// \note Does NOT check QueryConstruction(). It is assumed that the objectsThatExist list itself determines construction or destruction.
	/// \param[in] objectsThatExist Objects to check against
	/// \param[in] replicaManager ReplicaManager3 instance this connection is using
	virtual void SetConstructionByList(DataStructures::Multilist<ML_STACK, Replica3*> objectsThatExist, ReplicaManager3 *replicaManager);

	/// \internal
	/// \brief Given a list of objects that were created and destroyed, serialize and send them to another system.
	/// \param[in] newObjects Objects to serialize construction
	/// \param[in] deletedObjects Objects to serialize destruction
	/// \param[in] sendParameters Controlling parameters over the serialization
	/// \param[in] rakPeer Instance of RakPeerInterface to send on
	/// \param[in] worldId Which world, see ReplicaManager3::SetWorldID()
	virtual void SendConstruction(DataStructures::Multilist<ML_STACK, Replica3*> &newObjects, DataStructures::Multilist<ML_STACK, Replica3*> &deletedObjects, PRO sendParameters, RakPeerInterface *rakPeer, unsigned char worldId);

	/// \internal
	/// Remove from \a newObjectsIn objects that already exist and save to \a newObjectsOut
	/// Remove from \a deletedObjectsIn objects that do not exist, and save to \a deletedObjectsOut
	void CullUniqueNewAndDeletedObjects(DataStructures::Multilist<ML_STACK, Replica3*> &newObjectsIn,
		DataStructures::Multilist<ML_STACK, Replica3*> &deletedObjectsIn,
		DataStructures::Multilist<ML_STACK, Replica3*> &newObjectsOut,
		DataStructures::Multilist<ML_STACK, Replica3*> &deletedObjectsOut);

	/// \internal
	void SendValidation(RakPeerInterface *rakPeer, unsigned char worldId);


	// Internal - does the other system have this connection too? Validated means we can now use it
	bool isValidated;

protected:

	SystemAddress systemAddress;
	RakNetGUID guid;
	DataStructures::Multilist<ML_ORDERED_LIST, RakNetSmartPtr<LastSerializationResult>, Replica3*> constructedReplicas;
	friend class ReplicaManager3;
private:
	Connection_RM3() {};
};


/// Return codes for Connection_RM3::GetConstructionState() and Replica3::QueryConstruction()
/// Indicates what state the object should be in for the remote system
enum RM3ConstructionState
{
	/// This object should exist on the remote system. Send a construction message if necessary
	/// If the NetworkID is already in use, it will not do anything
	/// If it is not in use, it will create the object, and then call DeserializeConstruction
	RM3CS_SEND_CONSTRUCTION,

	/// This object should exist on the remote system.
	/// The other system already has the object, and the object will never be deleted.
	/// This is true of objects that are loaded with the level, for example.
	/// Treat it as if it existed, without sending a construction message.
	/// Will call SerializeConstructionExisting() to the object on the remote system
	RM3CS_ALREADY_EXISTS_REMOTELY,

	/// This object should not exist on the remote system. Send a destruction message if necessary.
	RM3CS_SEND_DESTRUCTION,

	/// Ignore this object.
	RM3CS_NO_ACTION,
};

/// Return codes when constructing an object
enum RM3SerializationResult
{
	/// This object serializes identically no matter who we send to
	/// We also send it to every connection (broadcast).
	/// Efficient for memory, speed, and bandwidth but only if the object is always broadcast identically.
	RM3SR_BROADCAST_IDENTICALLY,

	/// Either this object serializes differently depending on who we send to or we send it to some systems and not others.
	/// Inefficient for memory and speed, but efficient for bandwidth
	/// However, if you don't know what to return, return this
	RM3SR_SERIALIZED_UNIQUELY,

	/// Do not compare against last sent value. Just send no matter what
	/// Efficient for memory and speed, but not necessarily bandwidth
	RM3SR_SERIALIZED_ALWAYS,

	/// Do not serialize this object this tick, for this connection. Will query again next autoserialize timer
	RM3SR_DO_NOT_SERIALIZE,

	/// Never serialize this object for this connection
	/// Useful for objects that are downloaded, and never change again
	/// Efficient
	RM3SR_NEVER_SERIALIZE_FOR_THIS_CONNECTION,
};

enum RM3ActionOnPopConnection
{
	RM3AOPC_DO_NOTHING,
	RM3AOPC_DELETE_REPLICA,
	RM3AOPC_DELETE_REPLICA_AND_BROADCAST_DESTRUCTION,
};


/// \brief Base class for your replicated objects for the ReplicaManager3 system.
/// To use, derive your class, or a member of your class, from Replica3
/// Implement all pure virtual functions
class RAK_DLL_EXPORT Replica3 : public NetworkIDObject
{
public:
	Replica3();

	/// Before deleting a local instance of Replica3, call Replica3::BroadcastDestruction() for the deletion notification to go out on the network.
	/// It is not necessary to call ReplicaManager3::Dereference(), as this happens automatically in the destructor
	virtual ~Replica3();

	/// \brief Write a unique identifer for this class type
	/// The value written to \a allocationIdBitstream will be passed to Connection_RM3::AllocReplica()
	/// Sample implementation:
	/// {allocationIdBitstream->Write(RakNet::RakString("Soldier");}
	/// \param[out] allocationIdBitstream Bitstream for the user to write to, to identify this class
	virtual void WriteAllocationID(RakNet::BitStream *allocationIdBitstream) const=0;

	/// \brief This object does not exist on this system. Queried every tick to see if it should be created
	/// If ReplicaManager3::SetAutoManageConnections() is true, then this function is called by ReplicaManager3::Update() to determine if an object should exist on a given system. 
	/// If an object previously existed, but no longer does, a network message will be automatically sent to destroy it
	/// Likewise, if an object did not previously exist, but now does, a network message will be automatically sent to create it.
	/// \note Defaults are provided: QueryConstruction_PeerToPeer(), QueryConstruction_ServerConstruction(), QueryConstruction_ClientConstruction(). Return one of these functions for a working default for the relevant topology.
	/// \param[in] destinationConnection Which system we will send to
	/// \param[in] replicaManager3 Plugin instance for this Replica3
	/// \return What action to take
	virtual RM3ConstructionState QueryConstruction(RakNet::Connection_RM3 *destinationConnection, ReplicaManager3 *replicaManager3)=0;

	/// \brief This object already exists on this system. Queried every tick to see if it should still exist
	/// \param[in] destinationConnection Which system we will send to
	/// \param[in] replicaManager3 Plugin instance for this Replica3
	/// \return What action to take. Only RM3CS_SEND_DESTRUCTION does anything at this time.
	virtual RM3ConstructionState QueryDestruction(RakNet::Connection_RM3 *destinationConnection, ReplicaManager3 *replicaManager3) {(void) destinationConnection; (void) replicaManager3; return RM3CS_NO_ACTION;}

	/// \brief Should I allow this object to be created remotely, based solely on the sender?
	/// \note Defaults are provided: QueryRemoteConstruction_PeerToPeer(), QueryRemoteConstruction_ServerConstruction(), QueryRemoteConstruction_ClientConstruction(). Return one of these functions for a working default for the relevant topology.
	/// \param[in] sourceConnection Which system sent us the object creation request message.
	/// \return True to allow the object to pass onto DeserializeConstruction() (where it may also be rejected), false to immediately reject the remote construction request
	virtual bool QueryRemoteConstruction(RakNet::Connection_RM3 *sourceConnection)=0;

	/// \brief Write data to be sent only when the object is constructed on a remote system.
	/// SerializeConstruction is used to write out data that you need to create this object in the context of your game, such as health, score, name. Use it for data you only need to send when the object is created.
	/// After SerializeConstruction() is called, Serialize() will be called immediately thereafter. However, they are sent in different messages, so Serialize() may arrive a later frame than SerializeConstruction()
	/// \note The object's NetworkID and allocation id are handled by the system automatically, you do not need to write these values.
	/// \param[out] constructionBitstream Destination bitstream to write your data to
	/// \param[in] destinationConnection System that will receive this network message.
	virtual void SerializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *destinationConnection)=0;

	/// \brief Read data written by Replica3::SerializeConstruction() 
	/// Reads whatever data was written to \a constructionBitstream in Replica3::SerializeConstruction()
	/// \param[out] constructionBitstream Bitstream written to in Replica3::SerializeConstruction() 
	/// \param[in] sourceConnection System that sent us this network message.
	/// \return true to accept construction of the object. false to reject, in which case the object will be deleted via Replica3::DeallocReplica()
	virtual bool DeserializeConstruction(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *sourceConnection)=0;

	/// Same as SerializeConstruction(), but for an object that already exists on the remote system.
	/// Used if you return RM3CS_ALREADY_EXISTS_REMOTELY from QueryConstruction
	virtual void SerializeConstructionExisting(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *destinationConnection) {(void) constructionBitstream; (void) destinationConnection;};

	/// Same as DeserializeConstruction(), but for an object that already exists on the remote system.
	/// Used if you return RM3CS_ALREADY_EXISTS_REMOTELY from QueryConstruction
	virtual void DeserializeConstructionExisting(RakNet::BitStream *constructionBitstream, RakNet::Connection_RM3 *sourceConnection) {(void) constructionBitstream; (void) sourceConnection;};

	/// \brief Write extra data to send with the object deletion event, if desired
	/// Replica3::SerializeDestruction() will be called to write any object destruction specific data you want to send with this event.
	/// \a destructionBitstream can be read in DeserializeDestruction()
	/// \param[out] destructionBitstream Bitstream for you to write to
	/// \param[in] destinationConnection System that will receive this network message.
	virtual void SerializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *destinationConnection)=0;

	/// \brief Read data written by Replica3::SerializeDestruction() 
	/// Return true to delete the object. BroadcastDestruction() will be called automatically, followed by ReplicaManager3::Dereference.
	/// Return false to not delete it. If you delete it at a later point, you are responsible for calling BroadcastDestruction() yourself.
	virtual bool DeserializeDestruction(RakNet::BitStream *destructionBitstream, RakNet::Connection_RM3 *sourceConnection)=0;

	/// \brief The system is asking what to do with this replica when the connection is dropped
	virtual RM3ActionOnPopConnection QueryActionOnPopConnection(RakNet::Connection_RM3 *droppedConnection) const {(void) droppedConnection; return RM3AOPC_DELETE_REPLICA_AND_BROADCAST_DESTRUCTION;}

	/// \brief Override with {delete this;}
	/// 1. Got a remote message to delete this object which passed DeserializeDestruction(), OR
	/// 2. ReplicaManager3::SetAutoManageConnections() was called autoDestroy true (which is the default setting), and a remote system that owns this object disconnected) OR
	///
	/// Override with {delete this;} to actually delete the object (and any other processing you wish).
	/// If you don't want to delete the object, just do nothing, however, the system will not know this so you are responsible for deleting it yoruself later.
	/// destructionBitstream may be 0 if the object was deleted locally
	virtual void DeallocReplica(RakNet::Connection_RM3 *sourceConnection)=0;

	/// \brief Implement with QuerySerialization_ClientSerializable(), QuerySerialization_ServerSerializable(), or QuerySerialization_PeerToPeer()
	/// QuerySerialization() is a first pass query to check if a given object should serializable to a given system. The intent is that the user implements with one of the defaults for client, server, or peer to peer.
	/// Without this function, a careless implementation would serialize an object anytime it changed to all systems. This would give you feedback loops as the sender gets the same message back from the recipient it just sent to.
	/// If more than one system can serialize the same object then you will need to override to return true, and control the serialization result from Replica3::Serialize(). Be careful not to send back the same data to the system that just sent to you!
	/// \return True to allow calling Replica3::Serialize() for this connection, false to not call.
	virtual bool QuerySerialization(RakNet::Connection_RM3 *destinationConnection)=0;

	/// \brief Serialize our class to a bitstream
	/// User should implement this function to write the contents of this class to SerializationParamters::serializationBitstream
	/// If data only needs to be written once, you can write it to SerializeConstruction() instead for efficiency
	/// Transmitted over the network if it changed from the last time we called Serialize()
	/// Called every time the time interval to ReplicaManager3::SetAutoSerializeInterval() elapses and ReplicaManager3::Update is subsequently called.
	/// \param[in/out] serializeParameters Parameters controlling the serialization, including destination bitstream to write to
	/// \return Whether to serialize, and if so, how to optimize the results
	virtual RM3SerializationResult Serialize(RakNet::SerializeParameters *serializeParameters)=0;

	/// \brief Read what was written in Serialize()
	/// Reads the contents of the class from SerializationParamters::serializationBitstream
	/// Called whenever Serialize() is called with different data from the last send.
	/// \param[in] serializationBitstream Bitstream passed to Serialize()
	/// \param[in] timeStamp 0 if unused, else contains the time the message originated on the remote system
	/// \param[in] sourceConnection Which system sent to us
	virtual void Deserialize(RakNet::BitStream *serializationBitstream, RakNetTime timeStamp, RakNet::Connection_RM3 *sourceConnection)=0;

	/// If a system creates an object and NetworkIDManager::IsNetworkIDAuthority() returns false, then the object cannot locally assign NetworkID, which means that the object cannot be used over the network
	/// The object will call SerializeConstruction() and sent over the network with a temporary id.
	/// When the object is created by a system where NetworkIDManager::IsNetworkIDAuthority() returns true, SerializeConstructionRequestAccepted() will be called with the opportunity to write additional data if desired.
	/// The sender will then receive serializationBitstream in DeserializeConstructionRequestAccepted(), after the NetworkID has been assigned.
	/// This is not pure virtual, because it is not often used and is not necessary for the system to work.
	/// \param[out] serializationBitstream Destination bitstream to write to
	/// \param[in] requestingConnection Which system sent to us
	virtual void SerializeConstructionRequestAccepted(RakNet::BitStream *serializationBitstream, RakNet::Connection_RM3 *requestingConnection) {(void) serializationBitstream; (void) requestingConnection;}

	/// Receive the result of SerializeConstructionRequestAccepted
	/// \param[in] serializationBitstream Source bitstream to read from
	/// \param[in] acceptingConnection Which system sent to us
	virtual void DeserializeConstructionRequestAccepted(RakNet::BitStream *serializationBitstream, RakNet::Connection_RM3 *acceptingConnection) {(void) serializationBitstream; (void) acceptingConnection;}

	/// Same as SerializeConstructionRequestAccepted(), but the client construction request was rejected
	/// \param[out] serializationBitstream  Destination bitstream to write to
	/// \param[in] requestingConnection Which system sent to us
	virtual void SerializeConstructionRequestRejected(RakNet::BitStream *serializationBitstream, RakNet::Connection_RM3 *requestingConnection) {(void) serializationBitstream; (void) requestingConnection;}

	/// Receive the result of DeserializeConstructionRequestRejected
	/// \param[in] serializationBitstream Source bitstream to read from
	/// \param[in] requestingConnection Which system sent to us
	virtual void DeserializeConstructionRequestRejected(RakNet::BitStream *serializationBitstream, RakNet::Connection_RM3 *rejectingConnection) {(void) serializationBitstream; (void) rejectingConnection;}

	/// Called after DeserializeConstruction completes for the object successfully
	/// Override to trigger some sort of event when you know the object has completed deserialization.
	/// \param[in] sourceConnection System that sent us this network message.
	virtual void PostDeserializeConstruction(RakNet::Connection_RM3 *sourceConnection) {(void) sourceConnection;}

	/// Called after DeserializeDestruction completes for the object successfully, but obviously before the object is deleted
	/// Override to trigger some sort of event when you know the object has completed destruction.
	/// \param[in] sourceConnection System that sent us this network message.
	virtual void PreDestruction(RakNet::Connection_RM3 *sourceConnection) {(void) sourceConnection;}

	/// creatingSystemGUID is set the first time Reference() is called, or if we get the object from another system
	/// \return System that originally created this object
	RakNetGUID GetCreatingSystemGUID(void) const;

	/// Call to send a network message to delete this object on other systems
	/// Call it before deleting the object
	virtual void BroadcastDestruction(void);

	/// Default call for QueryConstruction(). Allow clients to create this object
	virtual RM3ConstructionState QueryConstruction_ClientConstruction(RakNet::Connection_RM3 *destinationConnection);
	/// Default call for QueryConstruction(). Allow clients to create this object
	virtual bool QueryRemoteConstruction_ClientConstruction(RakNet::Connection_RM3 *sourceConnection);

	/// Default call for QueryConstruction(). Allow the server to create this object, but not the client.
	virtual RM3ConstructionState QueryConstruction_ServerConstruction(RakNet::Connection_RM3 *destinationConnection);
	/// Default call for QueryConstruction(). Allow the server to create this object, but not the client.
	virtual bool QueryRemoteConstruction_ServerConstruction(RakNet::Connection_RM3 *sourceConnection);

	/// Default call for QueryConstruction(). Peer to peer - creating system sends the object to all other systems. No relaying.
	virtual RM3ConstructionState QueryConstruction_PeerToPeer(RakNet::Connection_RM3 *destinationConnection);
	/// Default call for QueryConstruction(). Peer to peer - creating system sends the object to all other systems. No relaying.
	virtual bool QueryRemoteConstruction_PeerToPeer(RakNet::Connection_RM3 *sourceConnection);

	/// Default call for QuerySerialization(). Use if the values you are serializing are generated by the client that owns the object. The serialization will be relayed through the server to the other clients.
	virtual bool QuerySerialization_ClientSerializable(RakNet::Connection_RM3 *destinationConnection);
	/// Default call for QuerySerialization(). Use if the values you are serializing are generated only by the server. The serialization will be sent to all clients, but the clients will not send back to the server.
	virtual bool QuerySerialization_ServerSerializable(RakNet::Connection_RM3 *destinationConnection);
	/// Default call for QuerySerialization(). Use if the values you are serializing are on a peer to peer network. The peer that owns the object will send to all. Remote peers will not send.
	virtual bool QuerySerialization_PeerToPeer(RakNet::Connection_RM3 *destinationConnection);

	/// GUID of the system that first called Reference() on this object.
	/// Transmitted automatically when the object is constructed
	RakNetGUID creatingSystemGUID;
	/// GUID of the system that caused the item to send a deletion command over the network
	RakNetGUID deletingSystemGUID;

	/// \internal
	/// ReplicaManager3 plugin associated with this object
	ReplicaManager3 *replicaManager;

	RakNetSmartPtr<LastSerializationResult> lastSentSerialization;
	bool forceSendUntilNextUpdate;
};


} // namespace RakNet


#endif

