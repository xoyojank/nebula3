#pragma once
//------------------------------------------------------------------------------
/**
    @class Characters::CharacterNode
  
    The CharacterNode class wraps a Character object into a ModelNode
    for rendering.
    
    (C) 2008 Radon Labs GmbH
*/  
#include "models/nodes/transformnode.h"
#include "characters/character.h"
#include "models/model.h"
#include "models/modelnodeinstance.h"
#include "coreanimation/animresource.h"
#include "coreanimation/managedanimresource.h"

#if NEBULA3_EDITOR
#include "COLLADAFWUniqueId.h"
#endif

//------------------------------------------------------------------------------
namespace Characters
{
class CharacterNode : public Models::TransformNode
{
    __DeclareClass(CharacterNode);
public:
    /// constructor
    CharacterNode();
    /// destructor
    virtual ~CharacterNode();

    /// called when attached to model node
    virtual void OnAttachToModel(const Ptr<Models::Model>& model);
    /// called when removed from model node
    virtual void OnRemoveFromModel();
    /// called when resources should be loaded
    virtual void LoadResources();
    /// called when resources should be unloaded
    virtual void UnloadResources();
    /// get overall state of contained resources (Initial, Loaded, Pending, Failed, Cancelled)
    virtual Resources::Resource::State GetResourceState() const;
    /// called once when all pending resource have been loaded
    virtual void OnResourcesLoaded();
    /// parse data tag (called by loader code)
    virtual bool ParseDataTag(const Util::FourCC& fourCC, const Ptr<IO::BinaryReader>& reader);

    /// create a model node instance
    virtual Ptr<Models::ModelNodeInstance> CreateNodeInstance() const;
    /// get the owned character object
    const Ptr<Character>& GetCharacter() const;
    /// get the character's managed animation resource
    const Ptr<CoreAnimation::ManagedAnimResource>& GetManagedAnimResource() const;

    /// set the character's animation resource
    void SetAnimationResourceId(const Resources::ResourceId& resId);
    /// get the character's animation resource
    const Resources::ResourceId& GetAnimationResourceId() const;

#if NEBULA3_EDITOR
	/// Set tje character's animation variation res
	void SetVariationResourceId(const Util::String& resId);
	/// add SkinList 
	void AddSkin(const Util::String& skinNodename);
	/// add joint
	void AddJoint(const COLLADAFW::UniqueId uniqueId,const CharacterJoint& joint);
	/// get joint
	CharacterJoint& GetJoint(const COLLADAFW::UniqueId& uniqueId);
	/// has joint by uniqueID
	bool HasJoint(const COLLADAFW::UniqueId& uniqueId);
	/// get joint num
	SizeT NumJoint() const;
	/// get joint indices
	Util::Array<IndexT> GetJointIndices();
	// write data to stream
	virtual bool WriteDataTag(Ptr<Models::ModelWriter>& writer);
#endif

private:
    /// recursively create model node instance and child model node instances
    virtual Ptr<Models::ModelNodeInstance> RecurseCreateNodeInstanceHierarchy(const Ptr<Models::ModelInstance>& modelInst, const Ptr<Models::ModelNodeInstance>& parentNodeInst=0);

    Resources::ResourceId animResId;
    Resources::ResourceId variationResId;
    Ptr<Character> character;
    Ptr<CoreAnimation::ManagedAnimResource> managedAnimResource;
    Ptr<CoreAnimation::ManagedAnimResource> managedVariationResource;

#if NEBULA3_EDITOR
	Util::Array<Util::String> skinList;
	Util::Dictionary<COLLADAFW::UniqueId,Characters::CharacterJoint> jointMaps;
#endif
};

//------------------------------------------------------------------------------
/**
*/
inline void
CharacterNode::SetAnimationResourceId(const Resources::ResourceId& resId)
{
    this->animResId = resId;
}

//------------------------------------------------------------------------------
/**
*/
inline const Resources::ResourceId&
CharacterNode::GetAnimationResourceId() const
{
    return this->animResId;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Character>&
CharacterNode::GetCharacter() const
{
    return this->character;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreAnimation::ManagedAnimResource>& 
CharacterNode::GetManagedAnimResource() const
{
    return this->managedAnimResource;
}

#if NEBULA3_EDITOR
//------------------------------------------------------------------------------
/**
*/
inline void
CharacterNode::AddSkin(const Util::String& skinNodename)
{
	this->skinList.Append(skinNodename);
}

//------------------------------------------------------------------------------
/**
*/
inline void
CharacterNode::AddJoint(const COLLADAFW::UniqueId uniqueId,const CharacterJoint& joint)
{
	this->jointMaps.Add(uniqueId,joint);
}

//------------------------------------------------------------------------------
/**
*/
inline CharacterJoint& 
CharacterNode::GetJoint(const COLLADAFW::UniqueId& uniqueId)
{	
	return this->jointMaps[uniqueId];
}

//------------------------------------------------------------------------------
/**
*/
inline bool
CharacterNode::HasJoint(const COLLADAFW::UniqueId& uniqueId)
{	
	return this->jointMaps.Contains(uniqueId);
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT 
CharacterNode::NumJoint() const
{
	return this->jointMaps.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline
Util::Array<IndexT> 
CharacterNode::GetJointIndices()
{
	Util::Array<IndexT> jointIndices;
	Util::Array<Characters::CharacterJoint> joints = this->jointMaps.ValuesAsArray();
	for (IndexT index=0; index< joints.Size();index++)
	{
		jointIndices.Append(joints[index].GetJointIndex());
	}
	return jointIndices;
}
#endif

} // namespace Characters
//------------------------------------------------------------------------------
  