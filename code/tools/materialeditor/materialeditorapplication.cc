//------------------------------------------------------------------------------
//  shadercompilerapplication.cc
//  (C) 2012 xoyojank
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "materialeditorapplication.h"
#include "system/win32/win32registry.h"
#include "qnodeeditor/qneblock.h"
#include "nsc3/shadernode.h"
#include "qnodeeditor/qneport.h"
#include "nsc3/shader.h"
#include "qnodeeditor/qneconnection.h"

namespace Tools
{
__ImplementSingleton(MaterialEditorApplication);

using namespace System;
using namespace Util;
using namespace IO;

int __placeholder = 0;
//------------------------------------------------------------------------------
/**
*/
MaterialEditorApplication::MaterialEditorApplication()
	: application(__placeholder, 0)
{
	__ConstructSingleton;
	this->shaderCompiler = ShaderCompiler::Create();
}

//------------------------------------------------------------------------------
/**
*/
MaterialEditorApplication::~MaterialEditorApplication()
{
	__DestructSingleton;
}

//------------------------------------------------------------------------------
/**
*/
bool
MaterialEditorApplication::ParseCmdLineArgs()
{
	this->filePattern = this->args.GetString("-shd", "*.xml");
	this->shaderCompiler->SetVerbose(this->args.GetBool("-verbose"));
	return true;
}

//------------------------------------------------------------------------------
/**
*/
void
MaterialEditorApplication::Run()
{
	this->application.exec();
}

//------------------------------------------------------------------------------
bool
MaterialEditorApplication::Open()
{
	if (ConsoleApplication::Open())
	{
		this->mainWindow.setWindowTitle(this->GetAppTitle().AsCharPtr());
		// parse command line args
		if (!this->ParseCmdLineArgs())
		{
			return false;
		}

		// load shader fragments and samplers
		this->shaderCompiler->SetProjectDirectory("home:");
		if (!this->shaderCompiler->LoadFragments())
		{
			this->SetReturnCode(10);
			return false;
		}
		if (!this->shaderCompiler->LoadSamplers())
		{
			this->SetReturnCode(10);
			return false;
		}
		this->mainWindow.show();
		this->mainWindow.SetupModels();
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
void
MaterialEditorApplication::Close()
{
	ConsoleApplication::Close();
}

//------------------------------------------------------------------------------
void
MaterialEditorApplication::New()
{

}

//------------------------------------------------------------------------------
bool
MaterialEditorApplication::Load(const Util::String& path)
{
	// load shader definitions
	return this->shaderCompiler->LoadShaders(path.ExtractFileName());
}

//------------------------------------------------------------------------------
void
MaterialEditorApplication::Save(const Util::String& path)
{

}

//------------------------------------------------------------------------------
bool
MaterialEditorApplication::Compile()
{
	// compile shaders
	bool result = this->shaderCompiler->Compile();
	if (result)
	{
		const Dictionary<String, Ptr<Shader>>& shaders = this->shaderCompiler->GetShaders();
		if (!shaders.IsEmpty())
		{
			const Ptr<Shader>& shader = shaders.ValueAtIndex(0);
			const Dictionary<String, Ptr<ShaderNode>>& shaderNodes = shader->GetShaderNodes();
			for (IndexT i = 0; i < shaderNodes.Size(); ++i)
			{
				const Ptr<ShaderNode>& shaderNode = shaderNodes.ValueAtIndex(i);
				const Dictionary<String, Ptr<ShaderSlot>>& inputSlots = shaderNode->GetInputSlots();
				const Dictionary<String, Ptr<ShaderSlot>>& outputSlots = shaderNode->GetOutputSlots();
				Array<Ptr<ShaderSlot>> typeSlots[ShaderSlot::NumSlotTypes];
				QNEBlock* block[ShaderSlot::NumSlotTypes] = {0};
				const char* SlotTypePostfix[ShaderSlot::NumSlotTypes] = { "(VS)", "(PS)", "" };
				for (IndexT j = 0; j < inputSlots.Size(); ++j)
				{
					const Ptr<ShaderSlot>& slot = inputSlots.ValueAtIndex(j);
					typeSlots[slot->GetSlotType()].Append(slot);
				}
				for (IndexT j = 0; j < outputSlots.Size(); ++j)
				{
					const Ptr<ShaderSlot>& slot = outputSlots.ValueAtIndex(j);
					typeSlots[slot->GetSlotType()].Append(slot);
				}
				for (IndexT j = 0; j < ShaderSlot::NumSlotTypes; ++j)
				{
					if (typeSlots[j].IsEmpty())
					{
						continue;
					}
					block[j] = new QNEBlock(0, this->mainWindow.GetScene());
					String name = shaderNode->GetName() + SlotTypePostfix[j];
					block[j]->addPort(name.AsCharPtr(), false, QNEPort::NamePort);
					block[j]->addPort(shaderNode->GetFragmentName().AsCharPtr(), false, QNEPort::TypePort);
				}
				for (IndexT j = 0; j < inputSlots.Size(); ++j)
				{
					const Ptr<ShaderSlot>& slot = inputSlots.ValueAtIndex(j);
					QNEPort* port = block[slot->GetSlotType()]->addPort(slot->GetName().AsCharPtr(), false);
					slot->SetUserData((ulong)port);
				}
				for (IndexT j = 0; j < outputSlots.Size(); ++j)
				{
					const Ptr<ShaderSlot>& slot = outputSlots.ValueAtIndex(j);
					QNEPort* port = block[slot->GetSlotType()]->addPort(slot->GetName().AsCharPtr(), true);
					slot->SetUserData((ulong)port);
				}
			}
			for (IndexT i = 0; i < shaderNodes.Size(); ++i)
			{
				const Ptr<ShaderNode>& shaderNode = shaderNodes.ValueAtIndex(i);
				const Dictionary<String, Ptr<ShaderSlot>>& outputSlots = shaderNode->GetOutputSlots();
				for (IndexT j = 0; j < outputSlots.Size(); ++j)
				{
					const Ptr<ShaderSlot>& slot = outputSlots.ValueAtIndex(j);
					QNEPort* output = (QNEPort*)slot->GetUserData();
					const Array<Ptr<ShaderSlot>>& connections = slot->GetConnections();
					for (IndexT k = 0; k < connections.Size(); ++k)
					{
						QNEPort* input = (QNEPort*)connections[k]->GetUserData();
						if (NULL != input)
						{
							QNEConnection* connection = new QNEConnection(0, this->mainWindow.GetScene());
							connection->setPort1(output);
							connection->setPort2(input);
							connection->updatePath();
						}
					}
				}
			}
		}

		return true;
	}
	return false;
}

} // namespace Tools
