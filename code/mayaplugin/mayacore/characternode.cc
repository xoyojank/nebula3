//------------------------------------------------------------------------------
//  characternode.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "mayacore/characternode.h"

namespace Characters
{
__ImplementClass(Characters::CharacterNode, 'CHRN', Models::TransformNode);

using namespace Util;
using namespace IO;


//------------------------------------------------------------------------------
/**
*/
CharacterNode::CharacterNode()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
CharacterNode::~CharacterNode()
{
}


//------------------------------------------------------------------------------
/**
*/
bool
CharacterNode::WriteDataTag(Ptr<ToolkitUtil::ModelWriter>& writer)
{
	writer->BeginTag("Animation", FourCC('ANIM'));
	writer->WriteString(this->animResId);
	writer->EndTag();

	this->character->WriteDataTag(writer);
	return TransformNode::WriteDataTag(writer);

    //bool retval = true;
    //if (FourCC('ANIM') == fourCC)
    //{
    //    // Animation
    //    this->animResId = reader->ReadString();
    //}
    //else if (FourCC('NJNT') == fourCC)
    //{
    //    // NumJoints
    //    SizeT numJoints = reader->ReadInt();
    //    this->character->Skeleton().Setup(numJoints);
    //}
    //else if (FourCC('JONT') == fourCC)
    //{
    //    // Joint
    //    IndexT jointIndex       = reader->ReadInt();
    //    IndexT parentJointIndex = reader->ReadInt();
    //    vector poseTranslation  = reader->ReadFloat4();
    //    quaternion poseRotation = quaternion(reader->ReadFloat4());
    //    vector poseScale        = reader->ReadFloat4();
    //    StringAtom jointName    = reader->ReadString();

    //    // FIXME: Maya likes to return quaternions with de-normalized numbers in it,
    //    // this should better be fixed by the asset pipeline!
    //    poseRotation.undenormalize();

    //    this->character->Skeleton().SetupJoint(jointIndex, parentJointIndex, poseTranslation, poseRotation, poseScale, jointName);
    //}
    //else if (FourCC('VART') == fourCC)
    //{
    //    // FIXME: Variations
    //    String variations = reader->ReadString();
    //}
    //else if (FourCC('NSKL') == fourCC)
    //{
    //    // NumSkinLists
    //    this->character->SkinLibrary().ReserveSkinLists(reader->ReadInt());
    //}
    //else if (FourCC('SKNL') == fourCC)
    //{
    //    // SkinList
    //    CharacterSkinList skinList;
    //    skinList.SetName(reader->ReadString());
    //    SizeT num = reader->ReadInt();
    //    IndexT i;
    //    Array<StringAtom> skins;
    //    for (i = 0; i < num; i++)
    //    {
    //        skins.Append(reader->ReadString());
    //    }
    //    skinList.SetSkins(skins);
    //    this->character->SkinLibrary().AddSkinList(skinList);
    //}
    //else
    //{
    //    retval = TransformNode::ParseDataTag(fourCC, reader);
    //}
    //return retval;
}

} // namespace Characters