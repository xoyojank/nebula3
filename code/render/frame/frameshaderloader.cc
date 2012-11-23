//------------------------------------------------------------------------------
//  frameshaderloader.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "frame/frameshaderloader.h"
#include "frame/framepass.h"
#include "frame/frameposteffect.h"
#include "io/ioserver.h"
#include "coregraphics/rendertarget.h"
#include "coregraphics/displaydevice.h"
#include "coregraphics/shaderserver.h"
#include "coregraphics/renderdevice.h"
#include "models/modelnodetype.h"
#include "resources/resourcemanager.h"
#if __PS3__
#include "coregraphics/ps3/ps3rendertarget.h"
#endif

namespace Frame
{
using namespace Resources;
using namespace CoreGraphics;
using namespace Models;
using namespace Util;
using namespace IO;

Util::Dictionary<StringAtom, ResourceId> FrameShaderLoader::textureNameResIdMapping;
//------------------------------------------------------------------------------
/**
*/
Ptr<FrameShader>
FrameShaderLoader::LoadFrameShader(const ResourceId& name, const URI& uri)
{
    Ptr<FrameShader> frameShader;
    Ptr<Stream> stream = IoServer::Instance()->CreateStream(uri);
    Ptr<XmlReader> xmlReader = XmlReader::Create();
    xmlReader->SetStream(stream);
    if (xmlReader->Open())
    {
        // make sure it's a valid frame shader file
        if (!xmlReader->HasNode("/Nebula3/FrameShader"))
        {
            n_error("FrameShaderLoader: '%s' is not a valid frame shader!", uri.AsString().AsCharPtr());
            return frameShader;
        }
        xmlReader->SetToNode("/Nebula3/FrameShader");
        frameShader = FrameShader::Create();
        frameShader->SetName(name);
        ParseFrameShader(xmlReader, frameShader);
        xmlReader->Close();
    }
    else
    {
        n_error("FrameShaderLoader: failed to load frame shader '%s'!", uri.AsString().AsCharPtr());
    }
    return frameShader;
}

//------------------------------------------------------------------------------
/**
*/
void
FrameShaderLoader::ParseFrameShader(const Ptr<XmlReader>& xmlReader, const Ptr<FrameShader>& frameShader)
{
    // parse render target declarations
    if (xmlReader->SetToFirstChild("DeclareRenderTarget")) do
    {
        ParseRenderTarget(xmlReader, frameShader);
    }
    while (xmlReader->SetToNextChild("DeclareRenderTarget")); 

    FrameShaderLoader::textureNameResIdMapping.Clear();
    // parse texture declarations
    if (xmlReader->SetToFirstChild("DeclareTexture")) do
    {
        ParseTexture(xmlReader, frameShader);
    }
    while (xmlReader->SetToNextChild("DeclareTexture"));

    // wait till all textures are loaded
    ResourceManager::Instance()->WaitForPendingResources(0.2);

    // parse multiple render target declarations
    if (xmlReader->SetToFirstChild("DeclareMRT")) do
    {
        ParseMultipleRenderTarget(xmlReader, frameShader);
    }
    while (xmlReader->SetToNextChild("DeclareMRT"));
        
    // parse frame passes and post effects, no other child types allowed from here!
    if (xmlReader->SetToFirstChild("Pass")
        || xmlReader->SetToFirstChild("PostEffect")) do
    {
        if (xmlReader->GetCurrentNodeName() == "Pass")
        {
            ParseFramePass(xmlReader, frameShader);
        }
        else
        {   
            n_assert(xmlReader->GetCurrentNodeName() == "PostEffect");
            ParsePostEffect(xmlReader, frameShader);
        }
    }
    while (xmlReader->SetToNextChild());
}

//------------------------------------------------------------------------------
/**
*/
void
FrameShaderLoader::ParseRenderTarget(const Ptr<XmlReader>& xmlReader, const Ptr<FrameShader>& frameShader)
{
    n_assert(DisplayDevice::Instance()->IsOpen());

    // create and configure a new render target
    Ptr<RenderTarget> renderTarget = RenderTarget::Create();
    const DisplayMode& displayMode = DisplayDevice::Instance()->GetDisplayMode();
    String name = xmlReader->GetString("name");
    renderTarget->SetResolveTextureResourceId(name);
    renderTarget->SetColorBufferFormat(PixelFormat::FromString(xmlReader->GetString("format")));
    if (xmlReader->HasAttr("depth"))
    {
        if (xmlReader->GetBool("depth"))
        {
            renderTarget->AddDepthStencilBuffer();
        }
    }
    if (xmlReader->HasAttr("depthTexture"))
    {
        n_assert2(xmlReader->HasAttr("depth") && xmlReader->GetBool("depth"), "Depthbuffer must be declared for a resolve!");
        renderTarget->SetResolveDepthTextureResourceId(xmlReader->GetString("depthTexture"));
    }
    if (xmlReader->HasAttr("sharedDepthBuffer"))
    {
        ResourceId depthBufferRTId = xmlReader->GetString("sharedDepthBuffer");
        if (!frameShader->HasRenderTarget(depthBufferRTId))
        {
            n_error("Invalid shared depth buffer name while parsing frame shader '%s'!\n", frameShader->GetName().Value());
        }
        renderTarget->AddSharedDepthStencilBuffer(frameShader->GetRenderTargetByName(depthBufferRTId));
    }
    if (xmlReader->HasAttr("width"))
    {
        renderTarget->SetWidth(xmlReader->GetInt("width"));
    }
    if (xmlReader->HasAttr("height"))
    {
        renderTarget->SetHeight(xmlReader->GetInt("height"));
    }
    if (xmlReader->HasAttr("relWidth"))
    {
		float relWidth = xmlReader->GetFloat("relWidth");
        renderTarget->SetWidth(uint(float(displayMode.GetWidth()) * relWidth));
		renderTarget->SetRelativeWidth(relWidth);
    }
    if (xmlReader->HasAttr("relHeight"))
    {
		float relHeight = xmlReader->GetFloat("relHeight");
        renderTarget->SetHeight(uint(float(displayMode.GetHeight()) * relHeight));
		renderTarget->SetRelativeHeight(relHeight);
    }
    if (xmlReader->HasAttr("msaa"))
    {
        if (xmlReader->GetBool("msaa"))
        {
            renderTarget->SetAntiAliasQuality(DisplayDevice::Instance()->GetAntiAliasQuality());
        }
    }
    if (xmlReader->HasAttr("cpuAccess"))
    {
        renderTarget->SetResolveTargetCpuAccess(xmlReader->GetBool("cpuAccess"));
    }
    // special case for PS3, if more extra handling is necessary make frameshaderloader platform specific
#if __PS3__
    if (xmlReader->HasAttr("bindToTile"))
    {
        renderTarget.cast<PS3::PS3RenderTarget>()->SetBindToTile(xmlReader->GetBool("bindToTile"));
    }
#endif
    // special case for xbox360, currently also usable by other platforms
    if (xmlReader->HasAttr("PrecededRenderTarget"))
    {
        Util::String previousRenderTarget = xmlReader->GetString("PrecededRenderTarget");
        const Ptr<RenderTarget>& pRenderTarget = frameShader->GetRenderTargetByName(previousRenderTarget);
        renderTarget->SetMemoryOffset(pRenderTarget->GetMemorySize());
    }
    renderTarget->Setup();
    frameShader->AddRenderTarget(name, renderTarget);
}

//------------------------------------------------------------------------------
/**
*/
Ptr<ShaderVariableInstance>
FrameShaderLoader::ParseShaderVariableInstance(const Ptr<XmlReader>& xmlReader, const Ptr<ShaderInstance>& shd)
{
    /// create a shader variable instance by semantic
    String semantic = xmlReader->GetString("sem");
    if (!shd->HasVariableBySemantic(semantic))
    {
        n_error("FrameShaderLoader: shader '%s' has no variable with semantic'%s' (%s, line %d)",
            shd->GetOriginalShader()->GetResourceId().Value(),
            semantic.AsCharPtr(),
            xmlReader->GetStream()->GetURI().AsString().AsCharPtr(),
            xmlReader->GetCurrentNodeLineNumber());
    }
    const Ptr<ShaderVariable>& shdVar = shd->GetVariableBySemantic(semantic);
    Ptr<ShaderVariableInstance> shdVarInst = shdVar->CreateInstance();

    /// get the default value of the shader variable
    String valueStr = xmlReader->GetString("value");
    switch (shdVarInst->GetShaderVariable()->GetType())
    {
        case ShaderVariable::IntType:
            shdVarInst->SetInt(valueStr.AsInt());
            break;

        case ShaderVariable::FloatType:
            shdVarInst->SetFloat(valueStr.AsFloat());
            break;

        case ShaderVariable::VectorType:
            shdVarInst->SetFloat4(valueStr.AsFloat4());
            break;

        case ShaderVariable::MatrixType:
            shdVarInst->SetMatrix(valueStr.AsMatrix44());
            break;

        case ShaderVariable::BoolType:
            shdVarInst->SetBool(valueStr.AsBool());
            break;

        case ShaderVariable::TextureType:
            {                  
                // rendertargets names are also theri resource ids, texture have a name -> resourceid mapping
                const Ptr<ResourceManager>& resManager = ResourceManager::Instance();
                if (resManager->HasResource(valueStr))
                {
                    const Ptr<Resource>& resource = resManager->LookupResource(valueStr);
                    shdVarInst->SetTexture(resource.downcast<Texture>()); 
                }
                else
                {
                    Util::String msg("FrameShader has no rendertarget or texture with name ");
                    msg.Append(valueStr);
                    n_assert2(FrameShaderLoader::textureNameResIdMapping.Contains(valueStr), msg.AsCharPtr());
                    ResourceId textureResId = FrameShaderLoader::textureNameResIdMapping[valueStr];
                    const Ptr<Resource>& resource = resManager->LookupResource(textureResId);
                    shdVarInst->SetTexture(resource.downcast<Texture>());       
                }
            }
            break;

        default:
            n_error("FrameShaderLoader::ParseShaderVariableInstance(): invalid shader variable type!\n");
            break;
    }
    return shdVarInst;
}

//------------------------------------------------------------------------------
/**
*/
void
FrameShaderLoader::ParseFramePass(const Ptr<XmlReader>& xmlReader, const Ptr<FrameShader>& frameShader)
{
    // create frame pass, optionally of requested class (default is FramePass)
    String framePassClass = xmlReader->GetOptString("class", "Frame::FramePass");
    Ptr<FramePass> framePass = (FramePass*) Core::Factory::Instance()->Create(framePassClass);
    n_assert(framePass->IsA(Frame::FramePass::RTTI));
    
    framePass->SetName(xmlReader->GetString("name"));

    // setup the pass shader
    ResourceId shdResId = ResourceId("shd:" + xmlReader->GetString("shader"));
    Ptr<ShaderInstance> shader = ShaderServer::Instance()->CreateShaderInstance(shdResId);
    framePass->SetShader(shader);
    
    // setup the render target (if not render to default render target)
    bool useDefaultRendertarget = true;
    if (xmlReader->HasAttr("renderTarget"))
    {
        n_assert2(!xmlReader->HasAttr("multipleRenderTarget"), "Rendertarget and multiple rendertarget are exclusivly!");
        useDefaultRendertarget = false;
        String rtName = xmlReader->GetString("renderTarget");
        if (frameShader->HasRenderTarget(rtName))
        {
            framePass->SetRenderTarget(frameShader->GetRenderTargetByName(rtName));
        }
        else
        {
            n_error("FrameShaderLoader: render target '%s' not declared (%s, line %d)",
                rtName.AsCharPtr(),
                xmlReader->GetStream()->GetURI().AsString().AsCharPtr(), 
                xmlReader->GetCurrentNodeLineNumber());
        }    
    }
    if (xmlReader->HasAttr("multipleRenderTarget"))
    {
        useDefaultRendertarget = false;
        String rtName = xmlReader->GetString("multipleRenderTarget");
        if (frameShader->HasMultipleRenderTarget(rtName))
        {
            framePass->SetMultipleRenderTarget(frameShader->GetMultipleRenderTargetByName(rtName));
        }
        else
        {
            n_error("FrameShaderLoader: multiple render target '%s' not declared (%s, line %d)",
                rtName.AsCharPtr(),
                xmlReader->GetStream()->GetURI().AsString().AsCharPtr(), 
                xmlReader->GetCurrentNodeLineNumber());
        } 
    }
    
    if (useDefaultRendertarget)
    {
        framePass->SetRenderTarget(RenderDevice::Instance()->GetDefaultRenderTarget());
    }

    // setup the clear color, depth and stencil (if defined)
    uint clearFlags = 0;
    if (xmlReader->HasAttr("clearColor"))
    {
        // clear color for all rendertargets
        framePass->SetClearColor(xmlReader->GetFloat4("clearColor"));
        clearFlags |= RenderTarget::ClearColor;
    }
    if (xmlReader->HasAttr("clearDepth"))
    {
        framePass->SetClearDepth(xmlReader->GetFloat("clearDepth"));
        clearFlags |= RenderTarget::ClearDepth;
    }
    if (xmlReader->HasAttr("clearStencil"))
    {
        framePass->SetClearStencil(uchar(xmlReader->GetInt("clearStencil")));
        clearFlags |= RenderTarget::ClearStencil;
    }
    framePass->SetClearFlags(clearFlags);

    // add shader variable instances
    if (xmlReader->SetToFirstChild("ApplyShaderVariable")) do
    {
        Ptr<ShaderVariableInstance> var = ParseShaderVariableInstance(xmlReader, shader);
        framePass->AddVariable(var);
    }
    while (xmlReader->SetToNextChild("ApplyShaderVariable"));

    // add render batches
    if (xmlReader->SetToFirstChild("Batch")) do
    {
        Ptr<FrameBatch> frameBatch = ParseFrameBatch(xmlReader, framePass->GetName().Value());
        framePass->AddBatch(frameBatch);
    }
    while (xmlReader->SetToNextChild("Batch"));

    // add frame pass to frame shader
    frameShader->AddFramePassBase(framePass.cast<FramePassBase>());

#if NEBULA3_ENABLE_PROFILING
    framePass->SetFramePassDebugTimer(Util::String("FramePass_") + framePass->GetName().Value());
#endif
}

//------------------------------------------------------------------------------
/**
*/
Ptr<FrameBatch>
FrameShaderLoader::ParseFrameBatch(const Ptr<XmlReader>& xmlReader, const Util::String& passName)
{
    Ptr<FrameBatch> frameBatch = FrameBatch::Create();
    
    // setup the batch shader
    ResourceId shdResId = ResourceId("shd:" + xmlReader->GetString("shader"));
    Ptr<ShaderInstance> shader = ShaderServer::Instance()->CreateShaderInstance(shdResId);
    frameBatch->SetShader(shader);

    // setup batch type, model node filter, lighting and sorting mode
    frameBatch->SetType(BatchType::FromString(xmlReader->GetString("type")));
    if (xmlReader->HasAttr("nodeFilter"))
    {
        frameBatch->SetNodeFilter(ModelNodeType::FromName(xmlReader->GetString("nodeFilter")));
    }
    if (xmlReader->HasAttr("lighting"))
    {
        frameBatch->SetLightingMode(LightingMode::FromString(xmlReader->GetString("lighting")));
    }
    if (xmlReader->HasAttr("sorting"))
    {
        frameBatch->SetSortingMode(SortingMode::FromString(xmlReader->GetString("sorting")));
    }
    if (xmlReader->HasAttr("shdFeatures"))
    {
        frameBatch->SetShaderFeatures(ShaderServer::Instance()->FeatureStringToMask(xmlReader->GetString("shdFeatures")));
    }

    // add shader variable instances
    if (xmlReader->SetToFirstChild("ApplyShaderVariable")) do
    {
        Ptr<ShaderVariableInstance> var = ParseShaderVariableInstance(xmlReader, shader);
        frameBatch->AddVariable(var);
    }
    while (xmlReader->SetToNextChild("ApplyShaderVariable"));

#if NEBULA3_ENABLE_PROFILING
    // add debug timer
    Util::String name;
    name.Format("FrameBatch:%s_%s_%s_%s_%s_%s_%s",
        passName.AsCharPtr(),
        xmlReader->GetString("shader").AsCharPtr(),
        xmlReader->GetString("type").AsCharPtr(),
        xmlReader->HasAttr("nodeFilter") ? xmlReader->GetString("nodeFilter").AsCharPtr() : "",
        xmlReader->HasAttr("lighting") ? xmlReader->GetString("lighting").AsCharPtr() : "",
        xmlReader->HasAttr("sorting") ? xmlReader->GetString("sorting").AsCharPtr() : "",
        xmlReader->HasAttr("shdFeatures") ? xmlReader->GetString("shdFeatures").AsCharPtr() : "");
    frameBatch->SetBatchDebugTimer(name);
#endif

    return frameBatch;
}

//------------------------------------------------------------------------------
/**
*/
void
FrameShaderLoader::ParsePostEffect(const Ptr<XmlReader>& xmlReader, const Ptr<FrameShader>& frameShader)
{
    // create and configure a new post effect
    Ptr<FramePostEffect> framePostEffect = FramePostEffect::Create();
    framePostEffect->SetName(xmlReader->GetString("name"));

    // setup the pass shader
    ResourceId shdResId = ResourceId("shd:" + xmlReader->GetString("shader"));
    Ptr<ShaderInstance> shader = ShaderServer::Instance()->CreateShaderInstance(shdResId);
    framePostEffect->SetShader(shader);

    // setup the render target (if not render to default render target)
    bool useDefaultRendertarget = true;
    if (xmlReader->HasAttr("renderTarget"))
    {
        n_assert2(!xmlReader->HasAttr("multipleRenderTarget"), "Rendertarget and multiple rendertarget are exclusivly!");
        useDefaultRendertarget = false;
        String rtName = xmlReader->GetString("renderTarget");
        if (frameShader->HasRenderTarget(rtName))
        {
            framePostEffect->SetRenderTarget(frameShader->GetRenderTargetByName(rtName));
        }
        else
        {
            n_error("FrameShaderLoader: render target '%s' not declared (%s, line %d)",
                rtName.AsCharPtr(),
                xmlReader->GetStream()->GetURI().AsString().AsCharPtr(), 
                xmlReader->GetCurrentNodeLineNumber());
        }    
    }
    if (xmlReader->HasAttr("multipleRenderTarget"))
    {
        useDefaultRendertarget = false;
        String rtName = xmlReader->GetString("multipleRenderTarget");
        if (frameShader->HasMultipleRenderTarget(rtName))
        {
            framePostEffect->SetMultipleRenderTarget(frameShader->GetMultipleRenderTargetByName(rtName));
        }
        else
        {
            n_error("FrameShaderLoader: mutliple render target '%s' not declared (%s, line %d)",
                rtName.AsCharPtr(),
                xmlReader->GetStream()->GetURI().AsString().AsCharPtr(), 
                xmlReader->GetCurrentNodeLineNumber());
        } 
    }

    if (useDefaultRendertarget)
    {
        framePostEffect->SetRenderTarget(RenderDevice::Instance()->GetDefaultRenderTarget());
    }

    // add shader variable instances
    if (xmlReader->SetToFirstChild("ApplyShaderVariable")) do
    {
        Ptr<ShaderVariableInstance> var = ParseShaderVariableInstance(xmlReader, shader);
        framePostEffect->AddVariable(var);
    }
    while (xmlReader->SetToNextChild("ApplyShaderVariable"));

    // add render batches
    if (xmlReader->SetToFirstChild("Batch")) do
    {
        Ptr<FrameBatch> frameBatch = ParseFrameBatch(xmlReader, framePostEffect->GetName().Value());
        framePostEffect->AddBatch(frameBatch);
    }
    while (xmlReader->SetToNextChild("Batch"));

    // setup frame post effect
    framePostEffect->Setup();
    // add post effect to frame shader
    frameShader->AddFramePassBase(framePostEffect.cast<FramePassBase>());
}

//------------------------------------------------------------------------------
/**
*/
void 
FrameShaderLoader::ParseMultipleRenderTarget(const Ptr<IO::XmlReader>& xmlReader, const Ptr<FrameShader>& frameShader)
{
    n_assert(DisplayDevice::Instance()->IsOpen());

    // create and configure a new render target
    Ptr<MultipleRenderTarget> multipleRenderTarget = MultipleRenderTarget::Create();
    String name = xmlReader->GetString("name");

    // collect all rendertargets 
    if (xmlReader->SetToFirstChild("RenderTarget")) do
    {
        String rtName = xmlReader->GetString("name");
        if (frameShader->HasRenderTarget(rtName))
        {
            multipleRenderTarget->AddRenderTarget(frameShader->GetRenderTargetByName(rtName));
            Math::float4 color(0.0f, 0.0f, 0.0f, 1.0f);
            if (xmlReader->HasAttr("clearColor"))
            {
                color = xmlReader->GetFloat4("clearColor");
            }
            multipleRenderTarget->SetClearColor(multipleRenderTarget->GetNumRendertargets() - 1, color);
            if (xmlReader->HasAttr("clearDepth"))
            {
                multipleRenderTarget->SetClearDepth(xmlReader->GetFloat("clearDepth"));
            }
            if (xmlReader->HasAttr("clearStencil"))
            {
                multipleRenderTarget->SetClearStencil((uchar)xmlReader->GetInt("clearStencil"));
            }
        }
        else
        {
            n_error("FrameShaderLoader: render target '%s' not declared (%s, line %d)",
                rtName.AsCharPtr(),
                xmlReader->GetStream()->GetURI().AsString().AsCharPtr(), 
                xmlReader->GetCurrentNodeLineNumber());
        }   
    }
    while (xmlReader->SetToNextChild("RenderTarget"));

    frameShader->AddMultipleRenderTarget(name, multipleRenderTarget);
}

//------------------------------------------------------------------------------
/**
*/
void 
FrameShaderLoader::ParseTexture(const Ptr<IO::XmlReader>& xmlReader, const Ptr<FrameShader>& frameShader)
{
    n_assert(DisplayDevice::Instance()->IsOpen());

    StringAtom name = xmlReader->GetString("name");    
    String res = xmlReader->GetString("resId") + NEBULA3_TEXTURE_EXTENSION;        
    Ptr<ManagedTexture> texture = ResourceManager::Instance()->CreateManagedResource(Texture::RTTI, res).downcast<ManagedTexture>();    
    frameShader->AddTexture(res, texture);           

    FrameShaderLoader::textureNameResIdMapping.Add(name, res);
}
} // namespace Frame