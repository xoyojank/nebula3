//------------------------------------------------------------------------------
//  animatornode.cc
//  (C) 2008 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/nodes/animatornode.h"
#include "models/nodes/animatornodeinstance.h"
#include "resources/resourcemanager.h"

namespace Models
{
__ImplementClass(Models::AnimatorNode, 'MANI', Models::ModelNode);

using namespace CoreAnimation;
using namespace Resources;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
AnimatorNode::AnimatorNode()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
AnimatorNode::~AnimatorNode()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Ptr<Models::ModelNodeInstance>
AnimatorNode::CreateNodeInstance() const
{
    Ptr<Models::ModelNodeInstance> newInst = (Models::ModelNodeInstance*) AnimatorNodeInstance::Create();
    return newInst;
}

//------------------------------------------------------------------------------
/**
    Unload animation resource if valid.
*/
void
AnimatorNode::UnloadAnimation()
{
    IndexT i;
    for (i = 0; i < this->animSection.Size(); i++)
    {
        if (this->animSection[i].managedAnimResource.isvalid())
        {
            ResourceManager::Instance()->DiscardManagedResource(this->animSection[i].managedAnimResource.upcast<ManagedResource>());
            this->animSection[i].managedAnimResource = 0;
        }
    }
}


//------------------------------------------------------------------------------
/**
    Load new animation, release old one if valid.
*/
bool
AnimatorNode::LoadAnimation()
{
    IndexT i;
    for (i = 0; i < this->animSection.Size(); i++)
    {
        if ((!this->animSection[i].managedAnimResource.isvalid()) && (!this->animSection[i].animationName.IsEmpty()))
        {
            // setup the managed resource
            this->animSection[i].managedAnimResource = ResourceManager::Instance()->CreateManagedResource(AnimResource::RTTI, this->animSection[i].animationName).downcast<ManagedAnimResource>();
            n_assert(this->animSection[i].managedAnimResource.isvalid());
        }
    }
    return true;
}

//------------------------------------------------------------------------------
/**
    Load the resources needed by this object.
*/
void
AnimatorNode::LoadResources()
{
    this->LoadAnimation();
    //this->resourcesValid = true;
}

//------------------------------------------------------------------------------
/**
    Unload the resources if refcount has reached zero.
*/
void
AnimatorNode::UnloadResources()
{
    //this->resourcesValid = false;
    this->UnloadAnimation();
}

//------------------------------------------------------------------------------
/**
*/
bool
AnimatorNode::ParseDataTag(const Util::FourCC& fourCC, const Ptr<IO::BinaryReader>& reader)
{
    bool retval = true;

    if (Util::FourCC('BASE') == fourCC)
    {
        AnimSection animSec;
        animSec.animationNodeType = (NodeType)reader->ReadInt(); 
        this->animSection.Append(animSec);
    }

    if (!this->animSection.IsEmpty())
    {
        AnimSection& curAnimSection = this->animSection.Back();
        AnimKey<Math::vector> val;
        Math::float4 vec;
        if (Util::FourCC('SLPT') == fourCC)
        {
            // LoopType
            Util::String s = reader->ReadString();
            if (s == "clamp")
            {
                curAnimSection.loopType = AnimLoopType::Clamp;
            }
            else if (s == "loop")
            {
                curAnimSection.loopType = AnimLoopType::Loop;
            }
            
        }
        else if (Util::FourCC('ANNO') == fourCC)
        {
            // AnimationPath
            curAnimSection.animatedNodesPath.Append(reader->ReadString());
        }
        else if ((Util::FourCC('SPNM') == fourCC) || (Util::FourCC('SVCN') == fourCC))
        {
            // ShaderVariable   
            Util::String semName = reader->ReadString();    
        #if __PS3__
            semName.ToUpper();
        #endif
            curAnimSection.shaderVarSemantic = semName;
        
        }
        else if (Util::FourCC('ADPK') == fourCC)
        {
            // PositionKey
            SizeT animKeySize = (SizeT)reader->ReadInt();
            for (IndexT animKeyIdx = 0; animKeyIdx < animKeySize; animKeyIdx++)
            {
                if (curAnimSection.animationNodeType == UvAnimator)   //uvAnimator
                {
                    int layer = reader->ReadInt();
                    val.SetTime(reader->ReadFloat());
                    float u = reader->ReadFloat();
                    float v = reader->ReadFloat();
                    vec.set(u, v, 0, 0);
                    val.SetValue(vec);
                    //skip other values - not needed for uvanimator
                    reader->ReadFloat(); reader->ReadFloat();
                    curAnimSection.layer.Append(layer);
                }
                else    //transformanimator
                {
                    val.SetTime(reader->ReadFloat());
                    float x = reader->ReadFloat();
                    float y = reader->ReadFloat();
                    float z = reader->ReadFloat();
                    float w = reader->ReadFloat();
                    vec.set(x, y, z, w);
                    val.SetValue(vec);        
                }
                curAnimSection.posArray.Append(val);
            }
        }
        else if (Util::FourCC('ADEK') == fourCC)
        {
            // eulerkey
            SizeT animKeySize = (SizeT)reader->ReadInt();
            for (IndexT animKeyIdx = 0; animKeyIdx < animKeySize; animKeyIdx++)
            {
                if (curAnimSection.animationNodeType == UvAnimator)   //uvAnimator
                {
                    int layer = reader->ReadInt();
                    val.SetTime(reader->ReadFloat());
                    float u = reader->ReadFloat();
                    float v = reader->ReadFloat();
                    vec.set(u, v, 0, 0);
                    val.SetValue(vec);
                    //skip other values - not needed for uvanimator
                    reader->ReadFloat(); reader->ReadFloat();
                    curAnimSection.layer.Append(layer);
                }
                else    //transformanimator
                {
                    val.SetTime(reader->ReadFloat());
                    float x = reader->ReadFloat();
                    float y = reader->ReadFloat();
                    float z = reader->ReadFloat();
                    float w = reader->ReadFloat();
                    vec.set(x, y, z, w);
                    val.SetValue(vec);        
                }
                curAnimSection.eulerArray.Append(val);
            }
        }
        else if (Util::FourCC('ADSK') == fourCC)
        {
            // ScaleKey
            SizeT animKeySize = (SizeT)reader->ReadInt();
            for (IndexT animKeyIdx = 0; animKeyIdx < animKeySize; animKeyIdx++)
            {
                if (curAnimSection.animationNodeType == UvAnimator)   //uvAnimator
                {
                    int layer = reader->ReadInt();
                    val.SetTime(reader->ReadFloat());
                    float u = reader->ReadFloat();
                    float v = reader->ReadFloat();
                    vec.set(u, v, 0, 0);
                    val.SetValue(vec);
                    //skip other values - not needed for uvanimator
                    reader->ReadFloat(); reader->ReadFloat();
                    curAnimSection.layer.Append(layer);
                }
                else    //transformanimator
                {
                    val.SetTime(reader->ReadFloat());
                    float x = reader->ReadFloat();
                    float y = reader->ReadFloat();
                    float z = reader->ReadFloat();
                    float w = reader->ReadFloat();
                    vec.set(x, y, z, w);
                    val.SetValue(vec);        
                }
                curAnimSection.scaleArray.Append(val);
            }
        }
        else if (Util::FourCC('SANI') == fourCC)
        {
            // Animation
            curAnimSection.animationName = reader->ReadString();
        }
        else if (Util::FourCC('SAGR') == fourCC)
        {
            // AnimationGroup
            curAnimSection.animationGroup = reader->ReadInt();
        }
        else if (Util::FourCC('ADDK') == fourCC)
        {
            Util::String valueType = reader->ReadString();
            SizeT animKeySize = (SizeT)reader->ReadInt();
            for (IndexT animKeyIdx = 0; animKeyIdx < animKeySize; animKeyIdx++)
            {
                float time = reader->ReadFloat();
                if (valueType == "Float")
                {
                    AnimKey<float> value;
                    value.SetTime(time);
                    value.SetValue(reader->ReadFloat());
                    curAnimSection.floatKeyArray.Append(value);
                }
                else if (valueType == "Float4")
                {
                    AnimKey<Math::float4> value;
                    value.SetTime(time);
                    value.SetValue(reader->ReadFloat4());
                    curAnimSection.float4KeyArray.Append(value);
                }
                else if (valueType == "Int")
                {
                    AnimKey<int> value;
                    value.SetTime(time);
                    value.SetValue(reader->ReadInt());
                    curAnimSection.intKeyArray.Append(value);
                }
                else
                {
                    n_error("AnimatorNode::ParseDataTag: Unkown key type!");
                }
            }
        }

        // UVAnimator, set shader
        if (curAnimSection.animationNodeType == UvAnimator)
        {
            curAnimSection.shaderVarSemantic = "TextureTransform0";
        }
    }
    return retval;
}

//------------------------------------------------------------------------------
/**
*/
Resources::Resource::State 
AnimatorNode::GetResourceState() const
{
    Resource::State state = ModelNode::GetResourceState();
    IndexT i;
    for (i = 0; i < this->animSection.Size(); ++i)
    {        
        if (this->animSection[i].managedAnimResource.isvalid()
            && this->animSection[i].managedAnimResource->GetState() > state)
        {
            state = this->animSection[i].managedAnimResource->GetState();
        }
    }
    
    return state;
}

#if NEBULA3_EDITOR
//------------------------------------------------------------------------------
/**
*/
bool 
AnimatorNode::AnimSection::WriteDataTag(Ptr<ModelWriter>& writer)
{
	writer->BeginTag("BeginAnimSection", FourCC('BASE'));
	writer->WriteInt((int)this->animationNodeType);

	/// loop type
	writer->BeginTag("LoopType", FourCC('SLPT'));
	writer->WriteString(AnimLoopType::ToString(this->loopType));
	writer->EndTag();

	for (IndexT pathIdx = 0; pathIdx < this->animatedNodesPath.Size(); pathIdx++)
	{
		writer->BeginTag("AnimatedNodes", FourCC('ANNO'));
		writer->WriteString(this->animatedNodesPath[pathIdx]);
		writer->EndTag();
	}	

	//writer->BeginTag("ParamName", FourCC('SPNM'));
	//writer->WriteString(this->paramName);
	//writer->EndTag();

	/// PositionKey
	if(!posArray.IsEmpty())
	{
		writer->BeginTag("PositionKey", FourCC('ADPK'));
		writer->WriteInt((int)posArray.Size());
		for( IndexT index = 0; index < posArray.Size();index++ )
		{
			if(this->loopType == AnimatorNode::UvAnimator)
			{
				writer->WriteInt(this->layer[index]);
				writer->WriteFloat(this->posArray[index].GetTime());
				writer->WriteFloat4(this->posArray[index].GetValue());
			}
			else
			{
				writer->WriteFloat(this->posArray[index].GetTime());
				writer->WriteFloat4(this->posArray[index].GetValue());
			}
		}
		writer->EndTag();
	}

	// eulerkey
	if(!eulerArray.IsEmpty())
	{
		writer->BeginTag("EulerKey", FourCC('ADEK'));
		writer->WriteInt((int)eulerArray.Size());
		for( IndexT index = 0; index < eulerArray.Size();index++ )
		{
			if(this->loopType == AnimatorNode::UvAnimator)
			{
				writer->WriteInt(this->layer[index]);
				writer->WriteFloat(this->eulerArray[index].GetTime());
				writer->WriteFloat4(this->eulerArray[index].GetValue());
			}
			else
			{
				writer->WriteFloat(this->eulerArray[index].GetTime());
				writer->WriteFloat4(this->eulerArray[index].GetValue());
			}
		}
		writer->EndTag();
	}

	// ScaleKey
	if(!scaleArray.IsEmpty())
	{
		writer->BeginTag("ScaleKey", FourCC('ADSK'));
		writer->WriteInt((int)scaleArray.Size());
		for( IndexT index = 0; index < scaleArray.Size();index++ )
		{
			if(this->loopType == AnimatorNode::UvAnimator)
			{
				writer->WriteInt(this->layer[index]);
				writer->WriteFloat(this->scaleArray[index].GetTime());
				writer->WriteFloat4(this->scaleArray[index].GetValue());
			}
			else
			{
				writer->WriteFloat(this->scaleArray[index].GetTime());
				writer->WriteFloat4(this->scaleArray[index].GetValue());
			}
		}
		writer->EndTag();
	}

	/// write group
	//writer->BeginTag("AnimationGroup", FourCC('SAGR'));
	//writer->WriteInt(this->animationGroup);
	//writer->EndTag();

	/// animation name
	//writer->BeginTag("AnimationName", FourCC('SANI'));
	//writer->WriteString(this->animationName);
	//writer->EndTag();

	/// write key
	//writer->BeginTag("Key", FourCC('ADDK'));
	//if( !float4KeyArray.IsEmpty())
	//{
	//	writer->WriteString("Float4");
	//	writer->WriteInt((int)float4KeyArray.Size());
	//	for( IndexT index = 0; index < float4KeyArray.Size(); index++ )
	//	{
	//		writer->WriteFloat4(float4KeyArray[index].GetValue());
	//	}
	//}
	//else if(!floatKeyArray.IsEmpty())
	//{
	//	writer->WriteString("Float");
	//	writer->WriteInt((int)floatKeyArray.Size());
	//	for( IndexT index = 0; index < floatKeyArray.Size(); index++ )
	//	{
	//		writer->WriteFloat(floatKeyArray[index].GetValue());
	//	}
	//}
	//else if(!intKeyArray.IsEmpty())
	//{
	//	writer->WriteString("Int");
	//	writer->WriteInt((int)intKeyArray.Size());
	//	for( IndexT index = 0; index < intKeyArray.Size(); index++ )
	//	{
	//		writer->WriteInt(intKeyArray[index].GetValue());
	//	}
	//}
	//writer->EndTag();	
	return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
AnimatorNode::WriteDataTag(Ptr<ModelWriter>& writer)
{
	IndexT i;    
	for (i = 0; i < this->animSection.Size(); i++)
	{
		AnimSection& animInfo = this->animSection[i];

		// catch errors
		if (((animInfo.animationNodeType == IntAnimator) ||
			(animInfo.animationNodeType == FloatAnimator) ||
			(animInfo.animationNodeType == Float4Animator))&&
			(animInfo.animationName.IsEmpty()))
		{
			continue;
		}

		animInfo.WriteDataTag(writer);
	}
	return true;
}
#endif
}