//------------------------------------------------------------------------------
//  transformnode.cc
//  (C) 2007 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "mayacore/transformnode.h"

namespace Models
{
	__ImplementClass(Models::TransformNode, 'TRFN', Models::ModelNode);

	using namespace Math;
	using namespace IO;
	using namespace Util;

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
	bool 
	TransformNode::WriteDataTag(Ptr<ToolkitUtil::ModelWriter>& writer)
	{
		if(ModelNode::WriteDataTag(writer))
		{
			// setposition
			//writer->BeginTag("Position", FourCC('POSI'));
			//writer->WriteFloat4(position);
			//writer->EndTag();

			//// rotation
			//writer->BeginTag("Rotation", FourCC('ROTN'));
			//writer->WriteFloat4(Math::float4(rotate.x(),rotate.y(),rotate.z(),rotate.w()));
			//writer->EndTag();

			//// scale
			//writer->BeginTag("Scale", FourCC('SCAL'));
			//writer->WriteFloat4(this->scale);
			//writer->EndTag();

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

	//------------------------------------------------------------------------------
	/**
	*/
	MStatus 
	TransformNode::ExtractNode(const MDagPath& nodePath)
	{
		return ModelNode::ExtractNode(nodePath);
	}
} // namespace Models
