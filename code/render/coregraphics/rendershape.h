#pragma once
//------------------------------------------------------------------------------
/**
    @class CoreGraphics::RenderShape
    
    Describes a shape which is rendered through the ShapeRenderer singleton.
    Shape rendering may be relatively inefficient and should only be used
    for debug visualizations.
    Please note that vertex and index data will be copied into a memory
    stream, so that it is safe to release or alter the original data
    once the shape object has been created. You have to be aware of
    the performance and memory-footprint implications though.
    Shape objects can be copied, but they will share the internal 
    vertex/index data copy.
    
    (C) 2008 Radon Labs GmbH
*/
#include "math/matrix44.h"
#include "math/float4.h"
#include "coregraphics/primitivetopology.h"
#include "coregraphics/indextype.h"
#include "io/memorystream.h"
#include "threading/threadid.h"

//------------------------------------------------------------------------------
namespace CoreGraphics
{
class RenderShape
{
public:
    /// shape types
    enum Type
    {
        Box,
        Sphere,
        Cylinder,
        Torus,
        Primitives,
        IndexedPrimitives,

        NumShapeTypes,
        InvalidShapeType,
    };

    /// default constructor
    RenderShape();
    /// shortcut constructor for simple shapes
    RenderShape(Threading::ThreadId threadId, Type shapeType, const Math::matrix44& modelTransform, const Math::float4& color);
    /// return true if object has been setup
    bool IsValid() const;
    /// setup simple shape
    void SetupSimpleShape(Threading::ThreadId threadId, Type shapeType, const Math::matrix44& modelTransform, const Math::float4& color);
    /// setup primitive batch (SLOW!)
    void SetupPrimitives(Threading::ThreadId threadId, const Math::matrix44& modelTransform, PrimitiveTopology::Code topology, SizeT numPrimitives, const void* vertices, SizeT vertexWidth, const Math::float4& color);
    /// setup indexed primitive batch (SLOW!)
    void SetupIndexedPrimitives(Threading::ThreadId threadId, const Math::matrix44& modelTransform, PrimitiveTopology::Code topology, SizeT numPrimitives, const void* vertices, SizeT numVertices, SizeT vertexWidth, const void* indices, IndexType::Code indexType, const Math::float4& color);

    /// get the thread id of this shape
    Threading::ThreadId GetThreadId() const;
    /// get shape type
    Type GetShapeType() const;
    /// get model transform
    const Math::matrix44& GetModelTransform() const;
    /// get primitive topology
    PrimitiveTopology::Code GetTopology() const;
    /// get number of primitives
    SizeT GetNumPrimitives() const;
    /// get pointer to vertex data (returns 0 if none exist)
    const void* GetVertexData() const;
    /// get vertex width in number of floats
    SizeT GetVertexWidth() const;
    /// get number of vertices (only for indexed primitives)
    SizeT GetNumVertices() const;
    /// get index data (returns 0 if none exists)
    const void* GetIndexData() const;
    /// get the index type (16 or 32 bit)
    IndexType::Code GetIndexType() const;
    /// get shape color
    const Math::float4& GetColor() const;

private:
    Threading::ThreadId threadId;
    Type shapeType;
    Math::matrix44 modelTransform;
    PrimitiveTopology::Code topology;
    SizeT numPrimitives;
    SizeT vertexWidth;
    SizeT numVertices;
    IndexType::Code indexType;
    Math::float4 color;
    IndexT vertexDataOffset;
    Ptr<IO::MemoryStream> dataStream;       // contains vertex/index data
};

//------------------------------------------------------------------------------
/**
*/
inline bool
RenderShape::IsValid() const
{
    return (InvalidShapeType != this->shapeType);
}

//------------------------------------------------------------------------------
/**
*/
inline Threading::ThreadId
RenderShape::GetThreadId() const
{
    return this->threadId;
}

//------------------------------------------------------------------------------
/**
*/
inline RenderShape::Type
RenderShape::GetShapeType() const
{
    return this->shapeType;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::matrix44&
RenderShape::GetModelTransform() const
{
    return this->modelTransform;
}

//------------------------------------------------------------------------------
/**
*/
inline PrimitiveTopology::Code
RenderShape::GetTopology() const
{
    n_assert((Primitives == this->shapeType) || (IndexedPrimitives == this->shapeType));
    return this->topology;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
RenderShape::GetNumPrimitives() const
{
    n_assert((Primitives == this->shapeType) || (IndexedPrimitives == this->shapeType));
    return this->numPrimitives;
}

//------------------------------------------------------------------------------
/**
*/
inline const void*
RenderShape::GetVertexData() const
{
    n_assert((Primitives == this->shapeType) || (IndexedPrimitives == this->shapeType));
    const void* ptr = ((uchar*)this->dataStream->GetRawPointer()) + this->vertexDataOffset;
    return ptr;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
RenderShape::GetVertexWidth() const
{
    n_assert((Primitives == this->shapeType) || (IndexedPrimitives == this->shapeType));
    return this->vertexWidth;
}

//------------------------------------------------------------------------------
/**
*/
inline SizeT
RenderShape::GetNumVertices() const
{
    n_assert(IndexedPrimitives == this->shapeType);
    return this->numVertices;
}

//------------------------------------------------------------------------------
/**
*/
inline const void*
RenderShape::GetIndexData() const
{
    n_assert(IndexedPrimitives == this->shapeType);
    const void* ptr = this->dataStream->GetRawPointer();
    return ptr;
}

//------------------------------------------------------------------------------
/**
*/
inline IndexType::Code
RenderShape::GetIndexType() const
{
    n_assert(IndexedPrimitives == this->shapeType);
    return this->indexType;
}

//------------------------------------------------------------------------------
/**
*/
inline const Math::float4&
RenderShape::GetColor() const
{
    return this->color;
}

} // namespace CoreGraphics
//------------------------------------------------------------------------------

