#pragma once
//------------------------------------------------------------------------------
/**
    @class Models::VisResolveContainer
    
    Helper class which keeps an array of visible nodes by type.
    
    (C) 2007 Radon Labs GmbH
*/
#include "core/ptr.h"
#include "models/modelnodetype.h"

//------------------------------------------------------------------------------
namespace Models
{
template<class TYPE> class VisResolveContainer
{
public:
    /// constructor
    VisResolveContainer();
    /// reset content
    void Reset();
    /// set the resolved flag for a given ModelNodeType
    void SetResolved(ModelNodeType::Code t, bool b);
    /// return true if the resolved flag has been set
    bool IsResolved(ModelNodeType::Code t) const;
    /// add a visible element by ModelNodeType
    void Add(IndexT frameIndex, ModelNodeType::Code t, const Ptr<TYPE>& e);
    /// get all visible elements of given ModelNodeType
    const Util::Array<Ptr<TYPE> >& Get(ModelNodeType::Code t) const;

private:
    struct Entry
    {
        /// constructor
        Entry() : resolved(false) {};

        Util::Array<Ptr<TYPE> > nodes;
        bool resolved;
    };
    Util::FixedArray<Entry> entries;
    IndexT frameIndex;
};

//------------------------------------------------------------------------------
/**
*/
template<class TYPE>
VisResolveContainer<TYPE>::VisResolveContainer() :
    entries(ModelNodeType::MaxNumModelNodeTypes),
    frameIndex(InvalidIndex)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
VisResolveContainer<TYPE>::SetResolved(ModelNodeType::Code t, bool b)
{
    this->entries[t].resolved = b;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> bool
VisResolveContainer<TYPE>::IsResolved(ModelNodeType::Code t) const
{
    return this->entries[t].resolved;
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
VisResolveContainer<TYPE>::Reset()
{
    IndexT i;
    for (i = 0; i < ModelNodeType::MaxNumModelNodeTypes; i++)
    {
        this->entries[i].resolved = false;
        this->entries[i].nodes.Clear();
    }
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> void
VisResolveContainer<TYPE>::Add(IndexT curFrameIndex, ModelNodeType::Code t, const Ptr<TYPE>& e)
{
    if (curFrameIndex != this->frameIndex)
    {
        this->Reset();
        this->frameIndex = curFrameIndex;
    }
    this->entries[t].nodes.Append(e);
}

//------------------------------------------------------------------------------
/**
*/
template<class TYPE> const Util::Array<Ptr<TYPE> >&
VisResolveContainer<TYPE>::Get(ModelNodeType::Code t) const
{
    return this->entries[t].nodes;
}

} // namespace Models
//------------------------------------------------------------------------------
    