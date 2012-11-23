//------------------------------------------------------------------------------
//  mayascenextractor.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include <maya/MItDag.h>
#include <maya/MItSelectionList.h>
#include <maya/MDagPath.h>
#include <maya/MGlobal.h>
#include <maya/MFnMesh.h>
#include "mayamain/mayalogger.h"
#include <mayacore/shapenode.h>
#include "mayaextractors/modelextractor.h"


namespace Models
{
	__ImplementClass(Models::ModelExtractor, 'MSCX', Models::ModelExtractorBase);

	using namespace Maya;
	using namespace ToolkitUtil;

	//------------------------------------------------------------------------------
	/**
	*/
	void
	ModelExtractor::Setup()
	{
		n_assert(!this->IsValid());
		ModelExtractorBase::Setup();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
	ModelExtractor::Discard()
	{
		n_assert(this->IsValid());
		ModelExtractorBase::Discard();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	ModelExtractor::Result
	ModelExtractor::Extract(const Ptr<Model>& model)
	{
		Ptr<ModelNode> transformNode = TransformNode::Create();
		transformNode->SetName("model");
		model->AttachNode(transformNode);

		n_assert(this->IsValid());
		if( this->isExportAll)
		{
			MayaLogger::Instance()->Print("Export the whole scene\n");
			MItDag dagIter;
			MFnDagNode worldDag (dagIter.root());
			MDagPath worldPath;
			worldDag.getPath(worldPath);
			stat = ExtractNode(transformNode,worldPath);
		}
		else
		{
			MayaLogger::Instance()->Print("Export selected objects\n");
			MSelectionList activeList;
			stat = MGlobal::getActiveSelectionList(activeList);
			if (MS::kSuccess != stat)
			{
				MayaLogger::Instance()->Error("Error retrieving selection list\n");
				return ModelExtractor::Error;
			}
			MItSelectionList iter(activeList);

			for ( ; !iter.isDone(); iter.next())
			{								
				MDagPath dagPath;
				stat = iter.getDagPath(dagPath);	
				stat = ExtractNode(transformNode,dagPath); 
			}							
		}
		return ModelExtractor::Success;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	MStatus 
	ModelExtractor::ExtractNode(const Ptr<ModelNode>& modeNode, const MDagPath& dagPath)
	{
		if (dagPath.hasFn(MFn::kMesh) && (dagPath.childCount() == 0))
		{	
			Ptr<ModelNode> shapeNode = ShapeNode::Create();
			if( shapeNode->ExtractNode(dagPath) == MS::kSuccess)
			{
				modeNode->AddChild(shapeNode);
			}
		}

		// look for meshes and cameras within the node's children
		for (unsigned int i=0; i<dagPath.childCount(); i++)
		{
			MObject child = dagPath.child(i);
			MDagPath childPath = dagPath;
			stat = childPath.push(child);
			if (MS::kSuccess != stat)
			{
				MayaLogger::Instance()->Error("Error retrieving path to child %d of: %s ",i,dagPath.fullPathName().asChar());
				return MS::kFailure;
			}
			stat = ExtractNode(modeNode,childPath);
			if (MS::kSuccess != stat)
				return MS::kFailure;
		}
		return MS::kSuccess;
	}

} // namespace Maya