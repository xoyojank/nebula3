#pragma once
//------------------------------------------------------------------------------
/**
    @class Win32::Win32CoreUIRenderer
  
    Win32 class for internal UI rendering.
    
    (C) 2008 Radon Labs GmbH
*/
#include "ui/base/coreuirendererbase.h"
#include "coregraphics/vertexbuffer.h"
#include "coregraphics/primitivegroup.h"
#include "coregraphics/texture.h"
#include "coregraphics/shadervariable.h"

namespace Win32
{
class Win360CoreUIRenderer : public Base::CoreUIRendererBase
{
    __DeclareClass(Win360CoreUIRenderer);
    __DeclareSingleton(Win360CoreUIRenderer);
public:
    /// Constructor
    Win360CoreUIRenderer();
    /// Destructor
    virtual ~Win360CoreUIRenderer();

    /// setup the renderer
    void Setup();
    /// discard the renderer
    void Discard();
    /// draw the accumulated ui
    void DrawElements(const Ptr<CoreGraphics::ShaderInstance>& shdInst);
    /// get gui shader instance
    const Ptr<CoreGraphics::ShaderInstance>& GetShader() const;

private:
    /// update vertex buffer with element geometry
    void UpdateVertexBuffer();

    struct DrawChunk
    {
        int startIndex;
        int numIndices;
        Ptr<Resources::ManagedTexture> texture;
    };
    Util::Array<DrawChunk> drawChunks;

    Ptr<CoreGraphics::VertexBuffer> vertexBuffer;
    CoreGraphics::ShaderVariable::Semantic guiTextureSemantic;
    CoreGraphics::PrimitiveGroup primGroup;
    SizeT curNumVertices;
    Ptr<CoreGraphics::ShaderInstance> guiShaderInst;
};

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::ShaderInstance>& 
Win360CoreUIRenderer::GetShader() const
{
    return this->guiShaderInst;
}

}; // namespace Win32