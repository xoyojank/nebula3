//------------------------------------------------------------------------------
//  statenode.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "mayacore/statenode.h"

namespace Models
{
__ImplementClass(Models::StateNode, 'STND', Models::TransformNode);

using namespace IO;
using namespace Util;
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
}

//------------------------------------------------------------------------------
/**
*/
bool
StateNode::WriteDataTag(Ptr<ToolkitUtil::ModelWriter>& writer)
{
	if(TransformNode::WriteDataTag(writer))
	{
		// shader
		writer->BeginTag("Shader", FourCC('SHDR'));
		writer->WriteString(this->shaderName);
		writer->EndTag();

		// model node type
		writer->BeginTag("ModelNodeType", FourCC('MNTP'));
		writer->WriteString(type);
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

//------------------------------------------------------------------------------
/**
    Manual shader parameters must be added before LoadResources is called, 
    because on LoadResources all shader params are validated.
*/
void 
StateNode::AddShaderParam(const Util::String& paramName, const Util::Variant& paramValue)
{
    Util::KeyValuePair<Util::StringAtom, Util::Variant> shaderParam(paramName, paramValue);
    this->shaderParams.Append(shaderParam);
}

//------------------------------------------------------------------------------
/**
*/
MStatus 
StateNode::ExtractNode(const MDagPath& nodePath)
{
	this->SetShader("shd:static");
	this->AddShaderParam("DiffMap0","textures:system/white");
	this->AddShaderParam("EmsvMap0","textures:system/black");
	this->AddShaderParam("SpecMap0","textures:system/white");
	this->AddShaderParam("BumpMap0","textures:system/nobump");
	return TransformNode::ExtractNode(nodePath);
}

} // namespace Models
