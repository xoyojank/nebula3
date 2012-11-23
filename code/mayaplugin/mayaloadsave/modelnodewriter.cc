//------------------------------------------------------------------------------
//  mayascenewriter.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "mayaloadsave/modelnodewriter.h"
#include "toolkitutil/xmlmodelwriter.h"
#include "toolkitutil/binarymodelwriter.h"

namespace Models
{
	__ImplementClass(Models::ModelNodeWriter, 'MONW', IO::StreamWriter);

	using namespace IO;
	using namespace Util;
	using namespace ToolkitUtil;

	//------------------------------------------------------------------------------
	/**
	*/
	ModelNodeWriter::ModelNodeWriter()
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	ModelNodeWriter::~ModelNodeWriter()
	{
		if (this->IsOpen())
		{
			this->Close();
		}
	}

	//------------------------------------------------------------------------------
	/**
	*/
	bool
	ModelNodeWriter::Open()
	{
		n_assert(!this->IsOpen());
		n_assert(this->stream.isvalid());
		n_assert(this->stream->CanWrite());

		if (StreamWriter::Open())
		{
			//this->modelWriter = XmlModelWriter::Create();
			this->modelWriter = BinaryModelWriter::Create();
			this->modelWriter->SetStream(this->stream);
			if (this->modelWriter->Open())
			{
				return true;
			}
		}
		return false;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
	ModelNodeWriter::Close()
	{
		n_assert(this->IsOpen());
		this->modelWriter->Close();
		this->modelWriter = 0;
		StreamWriter::Close();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
	ModelNodeWriter::WriteModel(const String& curModelName,const Ptr<Model>& model)
	{
		n_assert(this->IsOpen());

		String objClass = model->GetClassName();
		Util::FourCC objFourCC = model->GetClassFourCC();
		this->modelWriter->BeginModel(objClass, objFourCC, curModelName);
		const Util::Array<Ptr<ModelNode> >& nodes = model->GetNodes();
		for (int nodeIndex = 0; nodeIndex < nodes.Size(); nodeIndex++)
		{
			const Ptr<ModelNode>& modelNode = nodes[nodeIndex];
			this->WriteModelNode(modelNode);
		}
		this->modelWriter->EndModel();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
	ModelNodeWriter::WriteModelNode(const Ptr<ModelNode>& node)
	{
		String objClass = node->GetClassName();
		Util::StringAtom objName = node->GetName();
		Util::FourCC objFourCC = node->GetClassFourCC();
		this->modelWriter->BeginModelNode(objClass, objFourCC, objName.AsString());
		// Write DataTag
		node->WriteDataTag(this->modelWriter);

		// write child nodes
		const Util::Array<Ptr<ModelNode> >&nodes = node->GetChildren();
		for (int nodeIndex = 0; nodeIndex < nodes.Size(); nodeIndex++)
		{
			this->WriteModelNode(nodes[nodeIndex]);
		}
		this->modelWriter->EndModelNode();
	}
} // namespace Maya