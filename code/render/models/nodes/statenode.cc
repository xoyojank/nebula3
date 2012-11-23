//------------------------------------------------------------------------------
//  statenode.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/nodes/statenode.h"
#include "models/nodes/statenodeinstance.h"
#include "coregraphics/shaderserver.h"
#include "coregraphics/shadervariable.h"
#include "coregraphics/renderdevice.h"
#include "resources/resourcemanager.h"

namespace Models
{
__ImplementClass(Models::StateNode, 'STND', Models::TransformNode);

using namespace IO;
using namespace Util;
using namespace CoreGraphics;
using namespace Resources;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
StateNode::StateNode()
{
    // empty    
}

//------------------------------------------------------------------------------
/**
*/
StateNode::~StateNode()
{
    n_assert(!this->shaderInstance.isvalid());
    n_assert(this->managedTextureVariables.IsEmpty());
}

//------------------------------------------------------------------------------
/**
*/
Ptr<ModelNodeInstance>
StateNode::CreateNodeInstance() const
{
    Ptr<ModelNodeInstance> newInst = (ModelNodeInstance*) StateNodeInstance::Create();
    return newInst;
}

//------------------------------------------------------------------------------
/**
*/
bool
StateNode::ParseDataTag(const FourCC& fourCC, const Ptr<BinaryReader>& reader)
{
    bool retval = true;
    if (FourCC('SHDR') == fourCC)
    {
        // Shader
        this->SetShader(reader->ReadString());
    }
    else if (FourCC('STXT') == fourCC)
    {
        // ShaderTexture
        StringAtom paramName  = reader->ReadString();
        StringAtom paramValue = reader->ReadString();
        String fullTexResId = String(paramValue.AsString() + NEBULA3_TEXTURE_EXTENSION);
        this->shaderParams.Append(KeyValuePair<StringAtom,Variant>(paramName, Variant(fullTexResId)));
    }
    else if (FourCC('SINT') == fourCC)
    {
        // ShaderInt
        StringAtom paramName = reader->ReadString();
        int paramValue = reader->ReadInt();
        this->shaderParams.Append(KeyValuePair<StringAtom,Variant>(paramName, Variant(paramValue)));
    }
    else if (FourCC('SFLT') == fourCC)
    {
        // ShaderFloat
        StringAtom paramName = reader->ReadString();
        float paramValue = reader->ReadFloat();
        this->shaderParams.Append(KeyValuePair<StringAtom,Variant>(paramName, Variant(paramValue)));
    }
    else if (FourCC('SVEC') == fourCC)
    {
        // ShaderVector
        StringAtom paramName = reader->ReadString();
        float4 paramValue = reader->ReadFloat4();
        this->shaderParams.Append(KeyValuePair<StringAtom,Variant>(paramName, Variant(paramValue)));
    }
    else if (FourCC('STUS') == fourCC)
    {   
        // @todo: implement universal indexed shader parameters!
        // shaderparameter used by multilayered nodes
        int index = reader->ReadInt();
        float4 paramValue = reader->ReadFloat4();
        String paramName("MLPUVStretch");
        paramName.AppendInt(index);
        this->shaderParams.Append(KeyValuePair<StringAtom,Variant>(paramName, Variant(paramValue)));
    }
    else if (FourCC('SSPI') == fourCC)
    {     
        // @todo: implement universal indexed shader parameters!
        // shaderparameter used by multilayered nodes
        int index = reader->ReadInt();
        float4 paramValue = reader->ReadFloat4();
        String paramName("MLPSpecIntensity");
        paramName.AppendInt(index);
        this->shaderParams.Append(KeyValuePair<StringAtom,Variant>(paramName, Variant(paramValue)));
    }
    else
    {
        retval = TransformNode::ParseDataTag(fourCC, reader);
    }
    return retval;
}

//------------------------------------------------------------------------------
/**
*/
Resource::State
StateNode::GetResourceState() const
{
    // for resource streaming assume all textures are loaded
    return Resource::Loaded;

    //Resource::State state = TransformNode::GetResourceState();
    //IndexT i;
    //for (i = 0; i < this->managedTextureVariables.Size(); i++)
    //{
    //    const Ptr<ManagedTexture>& managedTexture = this->managedTextureVariables[i].managedTexture;
    //    if (managedTexture->GetState() > state)
    //    {
    //        state = managedTexture->GetState();
    //    }
    //}
    //return state;
}

//------------------------------------------------------------------------------
/**
*/
void
StateNode::LoadResources()
{
    n_assert(!this->shaderInstance.isvalid());
    n_assert(this->shaderResId.IsValid());
    ShaderServer* shdServer = ShaderServer::Instance();

    // create a new shader instance from the Shader attribute
    this->shaderInstance = shdServer->CreateShaderInstance(this->shaderResId);

    // iterate through our shader params and set them on the shader
    IndexT i;
    for (i = 0; i < this->shaderParams.Size(); i++)
    {
        const StringAtom& paramName = this->shaderParams[i].Key();
        const Variant& paramValue = this->shaderParams[i].Value();  
        
        // find the proper shader variable
        if (this->shaderInstance->HasVariableBySemantic(paramName))
        {
            const Ptr<ShaderVariable>& var = this->shaderInstance->GetVariableBySemantic(paramName);
            switch (var->GetType())
            {
                case ShaderVariable::IntType:
                    var->SetInt(paramValue.GetInt());
                    break;

                case ShaderVariable::FloatType:
                    var->SetFloat(paramValue.GetFloat());
                    break;

                case ShaderVariable::BoolType:
                    var->SetBool(paramValue.GetBool());
                    break;

                case ShaderVariable::VectorType:
                    var->SetFloat4(paramValue.GetFloat4());
                    break;

                case ShaderVariable::MatrixType:
                    var->SetMatrix(paramValue.GetMatrix44());
                    break;

                case ShaderVariable::TextureType:
                    this->SetupManagedTextureVariable(ResourceId(paramValue.GetString()), var);
                    break;

                default:
                    n_error("StateNode::LoadResources(): invalid shader variable type!\n");
                    break;
            }
        }
        else
        {
            /*
            n_printf("Warning: Invalid shader param '%s' in StateNode '%s' (shader='%s')\n",
                paramName.Value(),
                this->GetName().Value(),
                shaderInstance->GetOriginalShader()->GetResourceId().Value());
            */
        }
    }
    TransformNode::LoadResources();
}

//------------------------------------------------------------------------------
/**
*/
void
StateNode::UnloadResources()
{
    n_assert(this->shaderInstance.isvalid());

    // discard managed textures
    ResourceManager* resManager = ResourceManager::Instance();
    IndexT i;
    for (i = 0; i < this->managedTextureVariables.Size(); i++)
    {
        resManager->DiscardManagedResource(this->managedTextureVariables[i].managedTexture.upcast<ManagedResource>());
    }
    this->managedTextureVariables.Clear();

    // discard shader instance
    this->shaderInstance->Discard();
    this->shaderInstance = 0;
    ModelNode::UnloadResources();
}

//------------------------------------------------------------------------------
/**
    Create a new managed texture resource and bind it to the provided
    shader variable.
*/
void
StateNode::SetupManagedTextureVariable(const ResourceId& texResId, const Ptr<ShaderVariable>& var)
{
    // append textureextension dependend on platform
    Ptr<ManagedTexture> managedTexture = ResourceManager::Instance()->CreateManagedResource(Texture::RTTI, texResId).downcast<ManagedTexture>();
    this->managedTextureVariables.Append(ManagedTextureVariable(managedTexture, var));
}

//------------------------------------------------------------------------------
/**
    This method transfers texture from our managed texture objects into
    their associated shader variable. This is necessary since
    the actual texture of a managed texture may change from frame to frame
    because of resource management.
*/
void
StateNode::UpdateManagedTextureVariables(IndexT frameIndex)
{
    // @todo: don't update if contained texture hasn't changed,
    // should be a method of ManagedResource
    IndexT i;
    for (i = 0; i < this->managedTextureVariables.Size(); i++)
    {
        const Ptr<ManagedTexture>& tex = this->managedTextureVariables[i].managedTexture;
        const Ptr<ShaderVariable>& var = this->managedTextureVariables[i].shaderVariable;

        tex->UpdateRenderStats(this->resourceStreamingLevelOfDetail);

        // need to "touch" used resources for the resource management system
        if (tex->IsPlaceholder())
        {
            ResourceManager::Instance()->RequestResourceForLoading(tex.downcast<ManagedResource>());
        }
        else
        {
            tex->SetFrameId(frameIndex);
        }

        // set texture (or placeholder) to shader variable
        var->SetTexture(tex->GetTexture());
    }
}

//------------------------------------------------------------------------------
/**
*/
void
StateNode::ApplySharedState(IndexT frameIndex)
{
    // up to parent class
    TransformNode::ApplySharedState(frameIndex);

    // apply managed textures (actually contained texture may have changed)
    this->UpdateManagedTextureVariables(frameIndex);

    // set our shader instance as current
    ShaderServer::Instance()->SetActiveShaderInstance(this->shaderInstance);
}

//------------------------------------------------------------------------------
/**
    Manual shaderparameters must be added before LoadResources is called, 
    because on LoadResources all shaderparams are validated.
*/
void 
StateNode::AddShaderParam(const Util::String& paramName, const Util::Variant& paramValue)
{
    Util::KeyValuePair<Util::StringAtom, Util::Variant> shaderParam(paramName, paramValue);
    this->shaderParams.Append(shaderParam);
}

#if NEBULA3_EDITOR
//------------------------------------------------------------------------------
/**
*/
bool
StateNode::WriteDataTag(Ptr<ModelWriter>& writer)
{
	if(TransformNode::WriteDataTag(writer))
	{
		// shader
		writer->BeginTag("Shader", FourCC('SHDR'));
		writer->WriteString(this->shaderResId.AsString());
		writer->EndTag();

		// model node type
		writer->BeginTag("ModelNodeType", FourCC('MNTP'));
		writer->WriteString(ModelNodeType::ToName(type).AsString());
		writer->EndTag();

		for (IndexT index = 0; index < this->shaderParams.Size();index++ )
		{
			Variant& var = this->shaderParams[index].Value();
			if( var.GetType() == Variant::Int)
			{
				// Int
				writer->BeginTag("ShaderInt", FourCC('SINT'));
				writer->WriteString(this->shaderParams[index].Key().AsString());
				writer->WriteInt(var.GetInt());
				writer->EndTag();
			}
			else if( var.GetType() == Variant::Float)
			{
				writer->BeginTag("ShaderFloat", FourCC('SFLT'));
				writer->WriteString(this->shaderParams[index].Key().AsString());
				writer->WriteFloat(var.GetFloat());
				writer->EndTag();
			}
			else if( var.GetType() == Variant::Float4)
			{
				writer->BeginTag("ShaderVector", FourCC('SVEC'));
				writer->WriteString(this->shaderParams[index].Key().AsString());
				writer->WriteFloat4(var.GetFloat4());
				writer->EndTag();
			}
			else if( var.GetType() == Variant::String)
			{
				writer->BeginTag("ShaderTexture", FourCC('STXT'));
				writer->WriteString(this->shaderParams[index].Key().AsString());

				String textureName = var.GetString();
				textureName.StripFileExtension();
				writer->WriteString(textureName);
				writer->EndTag();
			}
			else
			{

			}
		}
		return true;
	}

	return false;
}
#endif

} // namespace Models
