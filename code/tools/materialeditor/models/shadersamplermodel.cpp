//------------------------------------------------------------------------------
//  shadersamplermodel.cpp
//  (C) 2012 xoyojank
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "shadersamplermodel.h"
#include "nsc3/shadersamplermanager.h"

//------------------------------------------------------------------------------
namespace Tools
{
using namespace Util;

//------------------------------------------------------------------------------
ShaderSamplerModel::ShaderSamplerModel(QObject *parent)
	: QStandardItemModel(parent)
{
	this->SetupItems();
}

//------------------------------------------------------------------------------
ShaderSamplerModel::~ShaderSamplerModel()
{

}

//------------------------------------------------------------------------------
QStringList
ShaderSamplerModel::mimeTypes() const
{
	return QStringList() << "application/x-qstandarditemmodeldatalist" << "text/uri-list";
}

//------------------------------------------------------------------------------
void
ShaderSamplerModel::AddItem(const QString& name)
{
	QIcon theIcon = QApplication::style()->standardIcon(QStyle::SP_DriveDVDIcon);

	QStandardItem* item = new QStandardItem(theIcon, name);
	item->setData(name);
	this->appendRow(item);
}

//------------------------------------------------------------------------------
void
ShaderSamplerModel::SetupItems()
{
	ShaderSamplerManager* samplerManager = ShaderSamplerManager::Instance();

	const Dictionary<String, Ptr<ShaderSampler>>& samplers = samplerManager->GetSamplers();
	for (IndexT i = 0; i < samplers.Size(); ++i)
	{
		const Ptr<ShaderSampler>& sampler = samplers.ValueAtIndex(i);
		QString name = sampler->GetName().AsCharPtr();
		this->AddItem(name);
	}
}

}// namespace Tools
//------------------------------------------------------------------------------