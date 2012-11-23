//------------------------------------------------------------------------------
//  debugshaperenderer.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "debugrender/debugshaperenderer.h"
#include "io/memorystream.h"
#include "debugrender/debugrenderprotocol.h"
#include "graphics/graphicsinterface.h"
#include "threading/thread.h"

namespace Debug
{
__ImplementClass(Debug::DebugShapeRenderer, 'DBSR', Core::RefCounted);
__ImplementSingleton(Debug::DebugShapeRenderer);

using namespace Util;
using namespace CoreGraphics;
using namespace Math;
using namespace IO;
using namespace Threading;

//------------------------------------------------------------------------------
/**
*/
DebugShapeRenderer::DebugShapeRenderer()
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
DebugShapeRenderer::~DebugShapeRenderer()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
DebugShapeRenderer::DrawBox(const matrix44& modelTransform, const float4& color)
{
    this->shapes.Append(RenderShape(Thread::GetMyThreadId(), RenderShape::Box, modelTransform, color));
}

//------------------------------------------------------------------------------
/**
*/
void
DebugShapeRenderer::DrawSphere(const matrix44& modelTransform, const float4& color)
{
    this->shapes.Append(RenderShape(Thread::GetMyThreadId(), RenderShape::Sphere, modelTransform, color));
}

//------------------------------------------------------------------------------
/**
*/
void
DebugShapeRenderer::DrawCylinder(const matrix44& modelTransform, const float4& color)
{
    this->shapes.Append(RenderShape(Thread::GetMyThreadId(), RenderShape::Cylinder, modelTransform, color));
}

//------------------------------------------------------------------------------
/**
*/
void
DebugShapeRenderer::DrawTorus(const matrix44& modelTransform, const float4& color)
{
    this->shapes.Append(RenderShape(Thread::GetMyThreadId(), RenderShape::Torus, modelTransform, color));
}

//------------------------------------------------------------------------------
/**
    NOTE: this method copies the vertex data to a temporary buffer.
*/
void
DebugShapeRenderer::DrawPrimitives(const matrix44& modelTransform, PrimitiveTopology::Code topology, SizeT numPrimitives, const void* vertices, SizeT vertexWidth, const float4& color)
{
    RenderShape shape;
    shape.SetupPrimitives(Thread::GetMyThreadId(), modelTransform, topology, numPrimitives, vertices, vertexWidth, color);
    this->shapes.Append(shape);
}

//------------------------------------------------------------------------------
/**
    NOTE: this method copies the vertex and index data to a temporary buffer.
*/
void
DebugShapeRenderer::DrawIndexedPrimitives(const matrix44& modelTransform, PrimitiveTopology::Code topology, SizeT numPrimitives, const void* vertices, SizeT numVertices, SizeT vertexWidth, const void* indices, IndexType::Code indexType, const float4& color)
{
    RenderShape shape;
    shape.SetupIndexedPrimitives(Thread::GetMyThreadId(), 
                                 modelTransform, 
                                 topology, 
                                 numPrimitives, 
                                 vertices, 
                                 numVertices, 
                                 vertexWidth, 
                                 indices, 
                                 indexType, 
                                 color);
    this->shapes.Append(shape);
}

//------------------------------------------------------------------------------
/**    
*/
void
DebugShapeRenderer::OnFrame()
{
    // send a RenderDebugShape message to the render thread, send this
    // every frame, whether there are any shapes or not as this will
    // also cleanup the shapes from the previous frame in the
    // render thread
    Ptr<RenderDebugShapes> msg = RenderDebugShapes::Create();
    msg->SetThreadId(Thread::GetMyThreadId());
    msg->SetShapes(this->shapes);
    Graphics::GraphicsInterface::Instance()->Send(msg.cast<Messaging::Message>());
    this->shapes.Clear();
}

} // namespace Debug
