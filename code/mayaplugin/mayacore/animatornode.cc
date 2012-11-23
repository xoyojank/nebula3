//------------------------------------------------------------------------------
//  animatornode.cc
//  (C) 2008 RadonLabs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "mayacore/animatornode.h"

namespace Models
{
__ImplementClass(Models::AnimatorNode, 'MANI', Models::ModelNode);

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
bool 
AnimatorNode::AnimSection::WriteDataTag(Ptr<ToolkitUtil::ModelWriter>& writer)
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

	writer->BeginTag("ParamName", FourCC('SPNM'));
	writer->WriteString(this->paramName);
	writer->EndTag();

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
	writer->BeginTag("AnimationGroup", FourCC('SAGR'));
	writer->WriteInt(this->animationGroup);
	writer->EndTag();

	/// animation name
	writer->BeginTag("AnimationName", FourCC('SANI'));
	writer->WriteString(this->animationName);
	writer->EndTag();

	/// write key
	writer->BeginTag("Key", FourCC('ADDK'));
	if( !float4KeyArray.IsEmpty())
	{
		writer->WriteString("Float4");
		writer->WriteInt((int)float4KeyArray.Size());
		for( IndexT index = 0; index < float4KeyArray.Size(); index++ )
		{
			writer->WriteFloat4(float4KeyArray[index].GetValue());
		}
	}
	else if(!floatKeyArray.IsEmpty())
	{
		writer->WriteString("Float");
		writer->WriteInt((int)floatKeyArray.Size());
		for( IndexT index = 0; index < floatKeyArray.Size(); index++ )
		{
			writer->WriteFloat(floatKeyArray[index].GetValue());
		}
	}
	else if(!intKeyArray.IsEmpty())
	{
		writer->WriteString("Int");
		writer->WriteInt((int)intKeyArray.Size());
		for( IndexT index = 0; index < intKeyArray.Size(); index++ )
		{
			writer->WriteInt(intKeyArray[index].GetValue());
		}
	}
	writer->EndTag();	
	return true;
}

//------------------------------------------------------------------------------
/**
*/
bool 
AnimatorNode::WriteDataTag(Ptr<ToolkitUtil::ModelWriter>& writer)
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

}