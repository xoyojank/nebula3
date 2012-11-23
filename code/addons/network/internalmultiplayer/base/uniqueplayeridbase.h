#pragma once
//------------------------------------------------------------------------------
/**
    @class InternalMultiplayer::UniquePlayerIdBase

    Information about sessions from a game search

(C) 2009 Radon Labs GmbH
*/    
//------------------------------------------------------------------------------
#include "core/types.h"
#include "internalmultiplayer/bitreader.h"
#include "internalmultiplayer/bitwriter.h"

namespace Base
{
class UniquePlayerIdBase
{
public:
    /// constructor
    UniquePlayerIdBase();
    /// destructor
    virtual ~UniquePlayerIdBase();
    /// encode id into a stream
    virtual void Encode(const Ptr<InternalMultiplayer::BitWriter>& writer) const;
    /// decode id from a stream
    virtual void Decode(const Ptr<InternalMultiplayer::BitReader>& reader);

protected:
};

} // namespace InternalMultiplayer
//-------------------