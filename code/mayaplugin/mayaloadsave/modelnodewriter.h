#pragma once
//------------------------------------------------------------------------------
/**
@class Maya::ModelNodeWriter

A stream writer which saves the content of a MayaScene object to
an XML-formatted stream.

(C) 2009 Radon Labs GmbH
*/
#include "io/streamwriter.h"
#include "mayacore/model.h"
#include "mayacore/modelnode.h"
#include "toolkitutil/xmlmodelwriter.h"

//------------------------------------------------------------------------------
namespace Models
{
	class ModelNodeWriter : public IO::StreamWriter
	{
		__DeclareClass(ModelNodeWriter);
	public:
		/// constructor
		ModelNodeWriter();
		/// destructor
		virtual ~ModelNodeWriter();

		/// begin writing to the stream
		virtual bool Open();
		/// end writing to the stream
		virtual void Close();
		/// write MayaScene object to the scene
		void WriteModel(const Util::String& curModelName,const Ptr<Model>& model);

	private:
		/// write a single Maya node
		void WriteModelNode(const Ptr<ModelNode>& node);
		/// write a single tag node
		//void WriteDataTag(const Ptr<ModelNode>& node);
		/// write an attribute of a Maya node
		//void WriteAttr(const Ptr<MayaNode>& mayaNode, IndexT attrIndex);

		Ptr<ToolkitUtil::ModelWriter> modelWriter;
	};

} // namespace Maya
//------------------------------------------------------------------------------
