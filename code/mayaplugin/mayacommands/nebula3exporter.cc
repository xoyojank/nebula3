//------------------------------------------------------------------------------
//  mayaattrutilcmd.cc
//  (C) 2009 Radon Labs GmbH
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "nebula3exporter.h"
#include "mayamain/mayalogger.h"
#include "mayacore/model.h"
#include "io/stream.h"
#include "io/ioserver.h"
#include "mayaextractors/modelextractor.h"
#include "mayaloadsave/modelnodewriter.h"

namespace Maya
{
using namespace IO;
using namespace Models;
//------------------------------------------------------------------------------
/**
*/
Nebula3Exporter::Nebula3Exporter()
{

}

//------------------------------------------------------------------------------
/**
*/
Nebula3Exporter::~Nebula3Exporter()
{
}

//------------------------------------------------------------------------------
/**
*/
void* Nebula3Exporter::creator()
{
	return new Nebula3Exporter();
}

//------------------------------------------------------------------------------
/**
*/
MStatus Nebula3Exporter::reader (const MFileObject& file,const MString& optionsString,FileAccessMode mode)
{
	MayaLogger::Instance()->Warning("Nebula3Exporter::reader called in error\n");
	return MS::kFailure;
}

//------------------------------------------------------------------------------
/**
*/
MStatus Nebula3Exporter::writer (const MFileObject& file,const MString& optionsString,FileAccessMode mode )
{
	MString mname = file.name();
	Util::String filename = "model_dst:";
	filename.Append(mname.asChar());
	Ptr<Model> model = Model::Create();

	Ptr<ModelExtractor> modelExtractor = ModelExtractor::Create();
	modelExtractor->Setup();
	if( ( mode == MPxFileTranslator::kExportAccessMode ) ||
		( mode == MPxFileTranslator::kSaveAccessMode ) )
	{
		modelExtractor->SetExportAll(true);
	}
	else if( mode == MPxFileTranslator::kExportActiveAccessMode )
	{
		modelExtractor->SetExportAll(false);
	}
	ModelExtractor::Result result = modelExtractor->Extract(model);

	if( ModelExtractor::Success == result)
	{
		Ptr<Stream> stream = IoServer::Instance()->CreateStream(filename);
		Ptr<ModelNodeWriter> writer = ModelNodeWriter::Create();
		writer->SetStream(stream);
		if (writer->Open())
		{
			writer->WriteModel("model",model);
			writer->Close();
		}
		MayaLogger::Instance()->Print("Nebula3Exporter::writer called success!\n");
		
	}

	modelExtractor->Discard();
	modelExtractor = 0;
	model = 0;
	if( ModelExtractor::Success == result)
	{
		return MS::kSuccess;
	}
	else
	{
		return MS::kFailure;
	}	
}
} // namespace Maya

