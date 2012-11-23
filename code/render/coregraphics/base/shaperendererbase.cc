//------------------------------------------------------------------------------
//  shaperendererbase.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "coregraphics/base/shaperendererbase.h"
#include "threading/threadid.h"

namespace Base
{
__ImplementClass(Base::ShapeRendererBase, 'SRBS', Core::RefCounted);
__ImplementSingleton(Base::ShapeRendererBase);

using namespace Math;
using namespace Util;
using namespace Threading;
using namespace CoreGraphics;

//------------------------------------------------------------------------------
/**
*/
ShapeRendererBase::ShapeRendererBase() :
    isOpen(false)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
ShapeRendererBase::~ShapeRendererBase()
{
    n_assert(!this->isOpen);
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeRendererBase::Open()
{
    n_assert(!this->isOpen);
    n_assert(this->shapes.IsEmpty());
    this->isOpen = true;
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeRendererBase::Close()
{
    n_assert(this->isOpen);
    this->isOpen = false;
    this->shapes.Clear();
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeRendererBase::DeleteShapesByThreadId(Threading::ThreadId threadId)
{
    n_assert(this->IsOpen());
    IndexT i;
    for (i = this->shapes.Size() - 1; i != InvalidIndex; i--)
    {
        ThreadId shapeThreadId = this->shapes[i].GetThreadId();
        n_assert(shapeThreadId != InvalidThreadId);
        if (shapeThreadId == threadId)
        {
            this->shapes.EraseIndex(i);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeRendererBase::AddShape(const RenderShape& shape)
{
    n_assert(this->IsOpen());
    this->shapes.Append(shape);
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeRendererBase::AddShapes(const Array<RenderShape>& shapeArray)
{
    n_assert(this->IsOpen());
    this->shapes.AppendArray(shapeArray);
}

//------------------------------------------------------------------------------
/**
*/
void
ShapeRendererBase::DrawShapes()
{
    // override in subclass!
    n_error("ShapeRendererBase::DrawShapes() called!");
}

//------------------------------------------------------------------------------
/**
*/
void 
ShapeRendererBase::AddWireFrameBox(const Math::bbox& boundingBox, const Math::float4& color, Threading::ThreadId threadId)
{
    // render lines around bbox
    const Math::point& center = boundingBox.center();
    const Math::vector& extends = boundingBox.extents();    
    const Math::vector corners[] = {  vector(1,1,1),
        vector(1,1,-1),
        vector(1,1,-1),
        vector(-1,1,-1),
        vector(-1,1,-1),
        vector(-1,1,1),
        vector(-1,1,1),
        vector(1,1,1),

        vector(1,-1,1),
        vector(1,-1,-1),
        vector(1,-1,-1),
        vector(-1,-1,-1),
        vector(-1,-1,-1),
        vector(-1,-1,1),
        vector(-1,-1,1),
        vector(1,-1,1),

        vector(1,1,1),
        vector(1,-1,1),
        vector(1,1,-1),
        vector(1,-1,-1),
        vector(-1,1,-1),
        vector(-1,-1,-1),
        vector(-1,1,1),
        vector(-1,-1,1)};

    Util::Array<point> lineList;
    IndexT i;
    for (i = 0; i < 24; ++i)
    {
        lineList.Append(center + float4::multiply(extends, corners[i]));    	
    }       
    RenderShape shape;
    shape.SetupPrimitives(threadId,
        matrix44::identity(),
        PrimitiveTopology::LineList,
        lineList.Size()/ 2,
        &(lineList.Front()),
        4,
        color);
    this->AddShape(shape);    
}
} // namespace Base
