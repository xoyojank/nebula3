#pragma once
//------------------------------------------------------------------------------
/**
    @class Characters::CharacterNode
  
    The CharacterNode class wraps a Character object into a ModelNode
    for rendering.
    
    (C) 2008 Radon Labs GmbH
*/  
#include "mayacore/transformnode.h"
#include "mayacore/character.h"

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

	// write data to stream
	virtual bool WriteDataTag(Ptr<ToolkitUtil::ModelWriter>& writer);
    /// get the owned character object
    const Ptr<Character>& GetCharacter() const;
    /// set the character's animation resource
    void SetAnimationResourceId(const Util::String& resId);

private:
	Util::String animResId;
    Ptr<Character> character;
};

//------------------------------------------------------------------------------
/**
*/
inline void
CharacterNode::SetAnimationResourceId(const Util::String& resId)
{
    this->animResId = resId;
}

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<Character>&
CharacterNode::GetCharacter() const
{
    return this->character;
}

} // namespace Characters
//------------------------------------------------------------------------------
  