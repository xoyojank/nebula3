#pragma once
//------------------------------------------------------------------------------
/**
@class Maya::MeshExtractor

This is mesh extractor which calls all other extractors.

(C) 2010 Dengwenyi@gmail.com
*/
#include "mayaextractors/modelextractorbase.h"
#include "mayacore/model.h"
#include <maya/MDagPathArray.h>
#include <maya/MFnDagNode.h>
#include <toolkitutil/meshutil/meshbuilder.h>

//------------------------------------------------------------------------------
namespace Models
{
	class MeshExtractor : public ModelExtractorBase
	{
		__DeclareClass(MeshExtractor);
	public:

		/// called to extract scene data
		MStatus ExtractNode(const Ptr<Model>& model, const MDagPath& nodePath);
		/// get mesh name
		const Util::String& GetMeshName() const ;
		/// get mesh builder
		ToolkitUtil::MeshBuilder& GetMeshBuilder();

	private:
		MStatus ExtractVertices(const MDagPath& meshDag);
		/// extract polygon
		MStatus ExtractPolygon(const MDagPath& meshDag);

	private:
		ToolkitUtil::MeshBuilder meshBuilder;
		Util::String meshName;
	};

	//------------------------------------------------------------------------------
	/**
	*/
	inline
	const Util::String& MeshExtractor::GetMeshName() const
	{
		return this->meshName;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline
	ToolkitUtil::MeshBuilder& MeshExtractor::GetMeshBuilder()
	{
		return this->meshBuilder;
	}

} // namespace Maya
//------------------------------------------------------------------------------
