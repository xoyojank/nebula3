#pragma once
//------------------------------------------------------------------------------
/**
    @class Frame::FrameShaderLoader
    
    Loader class to load frame shader from XML stream.
    
    (C) 2007 Radon Labs GmbH
*/
#include "frame/frameshader.h"
#include "io/uri.h"
#include "io/xmlreader.h"
#include "coregraphics/shadervariableinstance.h"
#include "coregraphics/shaderinstance.h"

//------------------------------------------------------------------------------
namespace Frame
{
class FrameShaderLoader
{
public:
    /// load a frame shader from an XML file
    static Ptr<FrameShader> LoadFrameShader(const Resources::ResourceId& name, const IO::URI& uri);

private:
    /// parse frame shader from XML
    static void ParseFrameShader(const Ptr<IO::XmlReader>& xmlReader, const Ptr<FrameShader>& frameShader);
    /// parse render target declaration from XML
    static void ParseRenderTarget(const Ptr<IO::XmlReader>& xmlReader, const Ptr<FrameShader>& frameShader);
    /// parse multiple render target declaration from XML
    static void ParseMultipleRenderTarget(const Ptr<IO::XmlReader>& xmlReader, const Ptr<FrameShader>& frameShader);
    /// parse texture declaration from XML
    static void ParseTexture(const Ptr<IO::XmlReader>& xmlReader, const Ptr<FrameShader>& frameShader);
    /// parse shader variable instance from XML
    static Ptr<CoreGraphics::ShaderVariableInstance> ParseShaderVariableInstance(const Ptr<IO::XmlReader>& xmlReader, const Ptr<CoreGraphics::ShaderInstance>& shd);
    /// parse frame pass from XML
    static void ParseFramePass(const Ptr<IO::XmlReader>& xmlReader, const Ptr<FrameShader>& frameShader);
    /// parse posteffect from XML
    static void ParsePostEffect(const Ptr<IO::XmlReader>& xmlReader, const Ptr<FrameShader>& frameShader);
    /// parse frame batch from XML
    static Ptr<FrameBatch> ParseFrameBatch(const Ptr<IO::XmlReader>& xmlReader, const Util::String& passName);
    /// array for mapping texture name in frame shader xml with resource id which is used in nebula3 for identifying textures
    static Util::Dictionary<Util::StringAtom, Resources::ResourceId> textureNameResIdMapping;
};

} // namespace Frame
//------------------------------------------------------------------------------
   