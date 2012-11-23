#pragma once
//------------------------------------------------------------------------------
/**
    @class Models::StateNode
  
    A model node which holds shader state information and applies shared shader
    state for its set of StateNodeInstances.
    
    (C) 2007 Radon Labs GmbH
*/
#include "models/nodes/transformnode.h"
#include "coregraphics/shaderinstance.h"
#include "resources/managedtexture.h"
#include "util/variant.h"

//------------------------------------------------------------------------------
namespace Models
{
class StateNode : public TransformNode
{
    __DeclareClass(StateNode);
public:
    /// constructor
    StateNode();
    /// destructor
    virtual ~StateNode();

    /// create a model node instance
    virtual Ptr<ModelNodeInstance> CreateNodeInstance() const;
    /// parse data tag (called by loader code)
    virtual bool ParseDataTag(const Util::FourCC& fourCC, const Ptr<IO::BinaryReader>& reader);
    /// called when resources should be loaded
    virtual void LoadResources();
    /// called when resources should be unloaded
    virtual void UnloadResources();
    /// get overall state of contained resources (Initial, Loaded, Pending, Failed, Cancelled)
    virtual Resources::Resource::State GetResourceState() const;
    /// apply state shared by all my ModelNodeInstances
    virtual void ApplySharedState(IndexT frameIndex);

    /// set shader resource id
    void SetShader(const Resources::ResourceId& resId);
    /// get shader resource id
    const Resources::ResourceId& GetShader() const;

    /// get pointer to contained shader instance
    const Ptr<CoreGraphics::ShaderInstance>& GetShaderInstance() const;
    /// add optional shader parameter, must be called before LoadResources
    void AddShaderParam(const Util::String& paramName, const Util::Variant& paramValue);
    /// get shaderparams
    const Util::Array<Util::KeyValuePair<Util::StringAtom, Util::Variant> >& GetShaderParameter() const;

#if NEBULA3_EDITOR
	// write data to stream
	virtual bool WriteDataTag(Ptr<ModelWriter>& writer);
#endif

protected:
    /// setup a new managed texture variable
    void SetupManagedTextureVariable(const Resources::ResourceId& texResId, const Ptr<CoreGraphics::ShaderVariable>& var);
    /// update managed texture variables
    void UpdateManagedTextureVariables(IndexT frameIndex);

    class ManagedTextureVariable
    {
    public:
        /// default constructor
        ManagedTextureVariable() {};
        /// constructor
        ManagedTextureVariable(const Ptr<Resources::ManagedTexture>& tex, const Ptr<CoreGraphics::ShaderVariable>& var) :
            managedTexture(tex),
            shaderVariable(var)
            { };

            Ptr<Resources::ManagedTexture> managedTexture;
            Ptr<CoreGraphics::ShaderVariable> shaderVariable;
    };

    Resources::ResourceId shaderResId;
    Ptr<CoreGraphics::ShaderInstance> shaderInstance;
    Util::Array<Util::KeyValuePair<Util::StringAtom, Util::Variant> > shaderParams;
    Util::Array<ManagedTextureVariable> managedTextureVariables;
};

//------------------------------------------------------------------------------
/**
*/
inline const Ptr<CoreGraphics::ShaderInstance>&
StateNode::GetShaderInstance() const
{
    return this->shaderInstance;
}

//------------------------------------------------------------------------------
/**
*/
inline void
StateNode::SetShader(const Resources::ResourceId& resId)
{
    this->shaderResId = resId;
}

//------------------------------------------------------------------------------
/**
*/
inline const Resources::ResourceId&
StateNode::GetShader() const
{
    return this->shaderResId;
}

//------------------------------------------------------------------------------
/**
*/
inline const Util::Array<Util::KeyValuePair<Util::StringAtom, Util::Variant> >& 
StateNode::GetShaderParameter() const
{
    return this->shaderParams;
}
} // namespace Models
//------------------------------------------------------------------------------
