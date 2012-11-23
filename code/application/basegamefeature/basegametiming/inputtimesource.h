#pragma once
//------------------------------------------------------------------------------
/**
    @class BaseGameFeature::InputTimeSource
  
    Provides a time source for input handling. This allows the Timing world
    to be paused independently from input handling.
    
    (C) 2007 Radon Labs GmbH
*/    
#include "basegametiming/timesource.h"
#include "core/singleton.h"

//------------------------------------------------------------------------------
namespace BaseGameFeature
{
class InputTimeSource : public TimeSource
{
    __DeclareClass(InputTimeSource);
    __DeclareSingleton(InputTimeSource);

public:
    /// constructor
    InputTimeSource();
    /// destructor
    virtual ~InputTimeSource();
};

} // namespace BaseGameFeature
//------------------------------------------------------------------------------
