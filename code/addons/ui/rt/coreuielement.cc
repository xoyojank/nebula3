//------------------------------------------------------------------------------
//  uirenderer.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "ui/rt/coreuielement.h"
#include "framesync/framesynctimer.h"

namespace CoreUI
{

//------------------------------------------------------------------------------
/**
*/
CoreUIElement::CoreUIElement() : 
    elementId(0),                                   
    color(1.0f, 1.0f, 1.0f, 1.0f),                  
    uv(0.0f, 0.0f, 1.0f, 1.0f),                     
    dimension(10, 10, 20, 20),                      
    depth(0)
{                                                                                
    // empty                                                                     
}                                                                                

//------------------------------------------------------------------------------
/**
*/
CoreUIElement::~CoreUIElement()
{	
	// empty
}

} // namespace Debug
