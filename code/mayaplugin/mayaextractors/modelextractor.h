#pragma once
//------------------------------------------------------------------------------
/**
@class Maya::MayaSceneExtractor

This is the top-level extractor which calls all other extractors.

(C) 2009 Radon Labs GmbH
*/
#include "mayaextractors/modelextractorbase.h"
#include "mayacore/model.h"
#include <maya/MDagPathArray.h>
#include <maya/MFnDagNode.h>
#include "mayacore/modelnode.h"

//------------------------------------------------------------------------------
namespace Models
{
	class ModelExtractor : public ModelExtractorBase
	{
		__DeclareClass(ModelExtractor);
	public:
		/// setup the object
		virtual void Setup();
		/// discard the objects
		virtual void Discard();

		/// called to extract scene data
		virtual Result Extract(const Ptr<Model>& model);

	private:
		/// extract a single node and recurse
		MStatus ExtractNode(const Ptr<ModelNode>& modeNode, const MDagPath& nodePath);

		MStatus stat;
	};

} // namespace Maya
//------------------------------------------------------------------------------
