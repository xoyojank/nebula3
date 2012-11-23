#pragma once
//------------------------------------------------------------------------------
/**
    @class Characters::CharacterSkinNode
    
    A ModelNode which wraps a character skin mesh.
    
    (C) 2008 Radon Labs GmbH
*/
#include "models/nodes/shapenode.h"

//------------------------------------------------------------------------------
namespace Characters
{
class CharacterSkinNode : public Models::ShapeNode
{
    __DeclareClass(CharacterSkinNode);
public:
    /// constructor
    CharacterSkinNode();
    /// destructor
    virtual ~CharacterSkinNode();
    
    /// create a model node instance
    virtual Ptr<Models::ModelNodeInstance> CreateNodeInstance() const;
    /// parse data tag (called by loader code)
    virtual bool ParseDataTag(const Util::FourCC& fourCC, const Ptr<IO::BinaryReader>& reader);
    /// apply state shared by all my ModelNodeInstances
    virtual void ApplySharedState(IndexT frameIndex);

#if NEBULA3_EDITOR
	// write data to stream
	virtual bool WriteDataTag(Ptr<Models::ModelWriter>& writer);
#endif

    /// reserve fragments (call before adding fragments)
    void ReserveFragments(SizeT numFragments);
    /// add a fragment (consisting of a mesh group index and a joint palette
    void AddFragment(IndexT primGroupIndex, const Util::Array<IndexT>& jointPalette);
    /// get number of skin fragments
    SizeT GetNumFragments() const;
    /// get primitive group index of a fragment
    IndexT GetFragmentPrimGroupIndex(IndexT fragmentIndex) const;
    /// get joint palette of a fragment
    const Util::Array<IndexT>& GetFragmentJointPalette(IndexT fragmentIndex) const;

private:
    /// a skin fragment
    class Fragment
    {
    public:
        /// constructor
        Fragment() : primGroupIndex(InvalidIndex) {};

        IndexT primGroupIndex;
        Util::Array<IndexT> jointPalette;
    };
    CoreGraphics::ShaderFeature::Mask skinnedShaderFeatureBits;
    Util::Array<Fragment> skinFragments;
};

//------------------------------------------------------------------------------
/**
*/
inline SizeT
CharacterSkinNode::GetNumFragments() const
{
    return this->skinFragments.Size();
}

//------------------------------------------------------------------------------
/**
*/
inline IndexT
CharacterSkinNode::GetFragmentPrimGroupIndex(IndexT fragmentIndex) const
{
    return this->skinFragments[fragmentIndex].primGroupIndex;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<IndexT>& 
CharacterSkinNode::GetFragmentJointPalette(IndexT fragmentIndex) const
{
    return this->skinFragments[fragmentIndex].jointPalette;
}

} // namespace Characters
//------------------------------------------------------------------------------
