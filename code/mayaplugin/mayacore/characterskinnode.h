#pragma once
//------------------------------------------------------------------------------
/**
    @class Characters::CharacterSkinNode
    
    A ModelNode which wraps a character skin mesh.
    
    (C) 2008 Radon Labs GmbH
*/
#include "mayacore/shapenode.h"

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

	// write data to stream
	virtual bool WriteDataTag(Ptr<ToolkitUtil::ModelWriter>& writer);

    /// reserve fragments (call before adding fragments)
    void ReserveFragments(SizeT numFragments);
    /// add a fragment (consisting of a mesh group index and a joint palette
    void AddFragment(IndexT primGroupIndex, const Util::Array<IndexT>& jointPalette);

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
    Util::Array<Fragment> skinFragments;
};

} // namespace Characters
//------------------------------------------------------------------------------
