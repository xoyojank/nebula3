//------------------------------------------------------------------------------
//  transformnode.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "models/nodes/transformnode.h"
#include "models/nodes/transformnodeinstance.h"

namespace Models
{
__ImplementClass(Models::TransformNode, 'TRFN', Models::ModelNode);

using namespace Math;
using namespace IO;
using namespace Util;
using namespace Resources;

//------------------------------------------------------------------------------
/**
*/
TransformNode::TransformNode() :
    position(0.0f, 0.0f, 0.0f),
    rotate(0.0f, 0.0f, 0.0f, 1.0f),
    scale(1.0f, 1.0f, 1.0f),
    rotatePivot(0.0f, 0.0f, 0.0f),
    scalePivot(0.0f, 0.0f, 0.0f),
    isInViewSpace(false),
    minDistance(0.0f),
    maxDistance(10000.0f),
    useLodDistances(false),
    lockedToViewer(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
TransformNode::~TransformNode()
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
Ptr<ModelNodeInstance>
TransformNode::CreateNodeInstance() const
{
    Ptr<ModelNodeInstance> newInst = (ModelNodeInstance*) TransformNodeInstance::Create();
    return newInst;
}

//------------------------------------------------------------------------------
/**
*/
Resource::State
TransformNode::GetResourceState() const
{
    return Resource::Loaded;
}

//------------------------------------------------------------------------------
/**
*/
bool 
TransformNode::ParseDataTag(const FourCC& fourCC, const Ptr<BinaryReader>& reader)
{
    bool retval = true;
    if (FourCC('POSI') == fourCC)
    {
        // position
        this->SetPosition(reader->ReadFloat4());
    }
    else if (FourCC('ROTN') == fourCC)
    {
        // rotation
        this->SetRotation(reader->ReadFloat4());
    }
    else if (FourCC('SCAL') == fourCC)
    {
        // scale
        this->SetScale(reader->ReadFloat4());
    }
    else if (FourCC('RPIV') == fourCC)
    {
        this->SetRotatePivot(reader->ReadFloat4());
    }
    else if (FourCC('SPIV') == fourCC)
    {
        this->SetScalePivot(reader->ReadFloat4());
    } 
    else if (FourCC('SVSP') == fourCC)
    {
        this->SetInViewSpace(reader->ReadBool());
    }
    else if (FourCC('SLKV') == fourCC)
    {
        this->SetLockedToViewer(reader->ReadBool());
    }
    else if (FourCC('SMID') == fourCC)
    {
        this->SetMinDistance(reader->ReadFloat());
    }
    else if (FourCC('SMAD') == fourCC)
    {
        this->SetMaxDistance(reader->ReadFloat());
    }
    else
    {
        retval = ModelNode::ParseDataTag(fourCC, reader);
    }
    return retval;
}

//------------------------------------------------------------------------------
/**
*/
void 
TransformNode::OnAttachToModel(const Ptr<Model>& model)
{
    ModelNode::OnAttachToModel(model);
    if (this->parent.isvalid() && this->parent->IsA(TransformNode::RTTI))
    {
        const Ptr<TransformNode>& parentTransformNode = this->parent.cast<TransformNode>();
        if (parentTransformNode->LodDistancesUsed())
        {
            this->SetMinDistance(parentTransformNode->GetMinDistance());
            this->SetMaxDistance(parentTransformNode->GetMaxDistance());
        }        
    }
}

#if NEBULA3_EDITOR
//------------------------------------------------------------------------------
/**
*/
bool 
TransformNode::WriteDataTag(Ptr<ModelWriter>& writer)
{
	if(ModelNode::WriteDataTag(writer))
	{
		// setposition
		if( this->position != Math::vector(0,0,0))
		{
			writer->BeginTag("Position", FourCC('POSI'));
			writer->WriteFloat4(this->position);
			writer->EndTag();
		}			

		// rotation
		if( this->rotate != Math::float4(0,0,0,1))
		{
			writer->BeginTag("Rotation", FourCC('ROTN'));
			writer->WriteFloat4(Math::float4(rotate.x(),rotate.y(),rotate.z(),rotate.w()));
			writer->EndTag();
		}			

		// scale
		if( this->scale != Math::vector(1,1,1))
		{
			writer->BeginTag("Scale", FourCC('SCAL'));
			writer->WriteFloat4(this->scale);
			writer->EndTag();
		}
		

		//writer->BeginTag("RotatePivot", FourCC('RPIV'));
		//writer->WriteFloat4(this->rotatePivot);
		//writer->EndTag();

		//writer->BeginTag("ScalePivot", FourCC('SPIV'));
		//writer->WriteFloat4(this->scalePivot);
		//writer->EndTag();

		//writer->BeginTag("RelativeToViewSpace", FourCC('SVSP')); 
		//writer->WriteBool(isInViewSpace);
		//writer->EndTag();

		//writer->BeginTag("LockViewer", FourCC('SLKV')); 
		//writer->WriteBool(lockedToViewer);
		//writer->EndTag();

		//writer->BeginTag("MinDistance", FourCC('SMID')); 
		//writer->WriteFloat(minDistance);
		//writer->EndTag();

		//writer->BeginTag("MaxDistance", FourCC('SMAD')); 
		//writer->WriteFloat(maxDistance);
		//writer->EndTag();

		return true;
	}		

	return false;		
}
#endif
} // namespace Models
