#pragma once
//------------------------------------------------------------------------------
/**
    @file graphicsattr/graphicsattributes.h    

    This is the central attribute registry for this feature. For more information
    on attributes, see Attr::Attribute.

    (C) 2007 Radon Labs GmbH
*/
#include "attr/attrid.h"
#include "attr/attributedefinition.h"

//------------------------------------------------------------------------------
namespace Attr
{
    // (simple) graphics property
    DeclareString(Graphics, 'GRFX', ReadWrite);        // name of the graphics resource
    
    // actor graphics property
    DeclareString(AnimSet, 'ANMS', ReadWrite);
    DeclareString(CharacterSet, 'CHRS', ReadWriter);
    DeclareString(StartAnimation, 'STAM', ReadWrite);
    DeclareString(IdleAnim, 'IDAN', ReadWrite);
    DeclareString(WalkAnim, 'WAAN', ReadWrite);
    DeclareBool(LoopStartAnimation, 'LOST', ReadWrite);
    
}; // namespace Attr
//------------------------------------------------------------------------------
