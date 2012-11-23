//------------------------------------------------------------------------------
//  win32coreuirenderer.cc
//  (C) 2008 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "ui/win360/win360coreuirenderer.h"
#include "coregraphics/memoryvertexbufferloader.h"
#include "coregraphics/renderdevice.h"
#include "coregraphics/shaderserver.h"
#include "coregraphics/transformdevice.h"

namespace Win32
{
__ImplementClass(Win360CoreUIRenderer, 'WUIR', Base::CoreUIRendererBase);
__ImplementSingleton(Win360CoreUIRenderer);

using namespace CoreGraphics;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
Win360CoreUIRenderer::Win360CoreUIRenderer():
curNumVertices(0)
{
    __ConstructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
Win360CoreUIRenderer::~Win360CoreUIRenderer()
{
    __DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
void
Win360CoreUIRenderer::Setup()
{
    Base::CoreUIRendererBase::Setup();

    // create vertex buffer
    this->vertexBuffer = VertexBuffer::Create();

    // MemoryVertexBufferLoader::Setup mit 0-Datapointern
    Util::Array<VertexComponent> vertexComponents;
    vertexComponents.Append(VertexComponent(VertexComponent::Position, 0, VertexComponent::Float3));
    vertexComponents.Append(VertexComponent(VertexComponent::Color, 0, VertexComponent::Float4));
    vertexComponents.Append(VertexComponent(VertexComponent::TexCoord, 0, VertexComponent::Float2));

    Ptr<MemoryVertexBufferLoader> vbLoader = MemoryVertexBufferLoader::Create();
    vbLoader->Setup(vertexComponents, 
                    4096, 
                    0, 
                    4096 * 9 * sizeof(float),
                    Base::ResourceBase::UsageDynamic,
                    Base::ResourceBase::AccessWrite);
    this->vertexBuffer->SetLoader(vbLoader.upcast<ResourceLoader>());
    this->vertexBuffer->SetAsyncEnabled(false);
    this->vertexBuffer->Load();
    n_assert(this->vertexBuffer->IsLoaded());
    this->vertexBuffer->SetLoader(0);

    // set primitive grout
    this->primGroup.SetBaseVertex(0);
    this->primGroup.SetNumVertices(4096);
    this->primGroup.SetBaseIndex(0);
    this->primGroup.SetNumIndices(0);

#if __XBOX360__
    this->primGroup.SetPrimitiveTopology(PrimitiveTopology::QuadList);
#else
    this->primGroup.SetPrimitiveTopology(PrimitiveTopology::TriangleList);
#endif

    // get the shader semantic
    this->guiTextureSemantic = ShaderVariable::Semantic("DiffMap0");

    // get gui shader for loading screen rendering
    ResourceId shdResId = ResourceId("shd:gui");
    this->guiShaderInst = ShaderServer::Instance()->CreateShaderInstance(shdResId);    
}

//------------------------------------------------------------------------------
/**
*/
void
Win360CoreUIRenderer::Discard()
{   
    if (this->vertexBuffer.isvalid())
    {
        this->vertexBuffer->Unload();
        this->vertexBuffer = 0;
    }
    if (this->guiShaderInst.isvalid())
    {
        this->guiShaderInst->Discard();
        this->guiShaderInst = 0;
    }
    Base::CoreUIRendererBase::Discard();
}

//------------------------------------------------------------------------------
/**
*/
void 
Win360CoreUIRenderer::UpdateVertexBuffer()
{
    float depthDivisor = this->sortedElements.ValueAtIndex(this->sortedElements.Size() - 1).GetDepth() + TINY;  

    // reset vertex buffer
    float* vertexPtr = (float*)this->vertexBuffer->Map(VertexBuffer::MapWriteDiscard);
    this->curNumVertices = 0;

    // now build the buffer
    // now do for all elements, but do it backwards, cause they are sorted from near to far, 
    // and we have to render them in other order
    IndexT index;
    for (index = 0; index < this->sortedElements.Size(); index++)
    {
        const CoreUI::CoreUIElement* curElement = &(this->sortedElements.ValueAtIndex(index));

        // skip elements where the texture is not in the texture pool, or the texture isn't loaded yet
        IndexT texIndex = this->texturePool.FindIndex(curElement->GetTextureResId());
        if (InvalidIndex == texIndex)
        {
            continue;
        }
        const Ptr<ManagedTexture>& texture = this->texturePool.ValueAtIndex(texIndex);
        if (texture->GetState() != Resources::Resource::Loaded)
        {
            continue;
        }   

        // setup new element
        DrawChunk drawChunk;
        drawChunk.texture = texture;
        drawChunk.startIndex = this->curNumVertices;

        // do as long, as the elements use the same texture and its not the last
        while (index < this->sortedElements.Size())
        {
            curElement = &(this->sortedElements.ValueAtIndex(index));

            // get element data to render
            Math::rectangle<float> rect = curElement->GetDimension();
            const Math::float4& uv = curElement->GetUV();
            Math::float4 color = curElement->GetColor();   
            float depth = float(curElement->GetDepth()) / depthDivisor;            

            // scale from 0..1 to -1..+1, invert Y
            // FIXME: move into vertex shader!
            rect.left   = (rect.left  * 2.0f) - 1.0f;
            rect.right  = (rect.right * 2.0f) - 1.0f;
            rect.top    = -((rect.top  * 2.0f) - 1.0f);
            rect.bottom = -((rect.bottom * 2.0f) - 1.0f);

            // first triangle, top-left
            *vertexPtr++ = rect.left; *vertexPtr++ = rect.top; *vertexPtr++ = depth;
            *vertexPtr++ = color.x(); *vertexPtr++ = color.y(); *vertexPtr++ = color.z(); *vertexPtr++ = color.w();
            *vertexPtr++ = uv.x(); *vertexPtr++ = uv.y();

            // first triangle, bottom-left
            *vertexPtr++ = rect.left; *vertexPtr++ = rect.bottom; *vertexPtr++ = depth;
            *vertexPtr++ = color.x(); *vertexPtr++ = color.y(); *vertexPtr++ = color.z(); *vertexPtr++ = color.w();
            *vertexPtr++ = uv.x(); *vertexPtr++ = uv.w();
#if !__XBOX360__
            // first triangle, top-right
            *vertexPtr++ = rect.right; *vertexPtr++ = rect.top; *vertexPtr++ = depth;
            *vertexPtr++ = color.x(); *vertexPtr++ = color.y(); *vertexPtr++ = color.z(); *vertexPtr++ = color.w();
            *vertexPtr++ = uv.z(); *vertexPtr++ = uv.y();

            // second triangle, bottom-left
            *vertexPtr++ = rect.left; *vertexPtr++ = rect.bottom; *vertexPtr++ = depth;
            *vertexPtr++ = color.x(); *vertexPtr++ = color.y(); *vertexPtr++ = color.z(); *vertexPtr++ = color.w();
            *vertexPtr++ = uv.x(); *vertexPtr++ = uv.w();
#endif
            // second triangle, bottom-right
            *vertexPtr++ = rect.right; *vertexPtr++ = rect.bottom; *vertexPtr++ = depth;
            *vertexPtr++ = color.x(); *vertexPtr++ = color.y(); *vertexPtr++ = color.z(); *vertexPtr++ = color.w();
            *vertexPtr++ = uv.z(); *vertexPtr++ = uv.w();

            // second triangle, top-right
            *vertexPtr++ = rect.right; *vertexPtr++ = rect.top; *vertexPtr++ = depth;
            *vertexPtr++ = color.x(); *vertexPtr++ = color.y(); *vertexPtr++ = color.z(); *vertexPtr++ = color.w();
            *vertexPtr++ = uv.z(); *vertexPtr++ = uv.y();
            
#if __XBOX360__
            this->curNumVertices += 4;
#else
            this->curNumVertices += 6;
#endif

            // check if the texture of the next element is different
            if (index < (this->sortedElements.Size() - 1))
            {
                const CoreUI::CoreUIElement& nextElement = this->sortedElements.ValueAtIndex(index + 1);
                if (nextElement.GetTextureResId() != curElement->GetTextureResId())
                {
                    break;
                }
            }
            
            // advance to next element
            index++;
        }

        // set end index
        drawChunk.numIndices = this->curNumVertices - drawChunk.startIndex;
        this->drawChunks.Append(drawChunk);       
    }

    // close vertex buffer
    this->vertexBuffer->Unmap();
}

//------------------------------------------------------------------------------
/**
*/
void 
Win360CoreUIRenderer::DrawElements(const Ptr<CoreGraphics::ShaderInstance>& shdInst)
{
    //in case of this assertion: increase the number of vertices (4096) in open-function
    n_assert((this->sortedElements.Size() * 6) < this->vertexBuffer->GetNumVertices());  
    
    // just do if one element min
    if (this->sortedElements.IsEmpty())
    {
        return;
    }

    // update the vertex buffer
    this->UpdateVertexBuffer();

    // now simply draw the vertex buffer
    IndexT index;
    for (index = 0; index < this->drawChunks.Size(); index++)
    {
        const DrawChunk& drawChunk = this->drawChunks[index];

        //// apply shader variables
        shdInst->GetVariableBySemantic(this->guiTextureSemantic)->SetTexture(drawChunk.texture->GetTexture());
        shdInst->Commit();

        //// render the vertex buffer
        RenderDevice* renderDevice = RenderDevice::Instance();        
        this->primGroup.SetBaseVertex(drawChunk.startIndex);
        this->primGroup.SetNumVertices(drawChunk.numIndices);
        renderDevice->SetStreamSource(0, this->vertexBuffer, 0);
        renderDevice->SetVertexLayout(this->vertexBuffer->GetVertexLayout());
        renderDevice->SetPrimitiveGroup(this->primGroup);
        renderDevice->Draw();
    }
    this->drawChunks.Clear();
}

} // namespace Win32
