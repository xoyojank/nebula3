//------------------------------------------------------------------------------
//  characterskinnode.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "mayacore/characterskinnode.h"

namespace Characters
{
__ImplementClass(Characters::CharacterSkinNode, 'CHSN', Models::ShapeNode);

using namespace Util;
using namespace Models;
using namespace IO;

//------------------------------------------------------------------------------
/**
*/
CharacterSkinNode::CharacterSkinNode()
{
}

//------------------------------------------------------------------------------
/**
*/
CharacterSkinNode::~CharacterSkinNode()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
bool
CharacterSkinNode::WriteDataTag(Ptr<ToolkitUtil::ModelWriter>& writer)
{
	writer->BeginTag("NumSkinFragments", 'NSKF');
	writer->WriteInt(skinFragments.Size());
	writer->EndTag();

	for (IndexT index = 0 ; index < this->skinFragments.Size();index++ )
	{
		writer->BeginTag("SkinFragment", 'SFRG');
		writer->WriteInt(this->skinFragments[index].primGroupIndex);
		writer->WriteIntArray(this->skinFragments[index].jointPalette);
		writer->EndTag();
	}

	return ShapeNode::WriteDataTag(writer);
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterSkinNode::ReserveFragments(SizeT numFragments)
{
    this->skinFragments.Clear();
    this->skinFragments.Reserve(numFragments);
}

//------------------------------------------------------------------------------
/**
*/
void
CharacterSkinNode::AddFragment(IndexT primGroupIndex, const Util::Array<IndexT>& jointPalette)
{
    Fragment fragment;
    this->skinFragments.Append(fragment);
    this->skinFragments.Back().primGroupIndex = primGroupIndex;
    this->skinFragments.Back().jointPalette = jointPalette;
}

} // namespace Characters
