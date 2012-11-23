//------------------------------------------------------------------------------
//  shaderfragmentmodel.cpp
//  (C) 2012 xoyojank
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "shaderfragmentmodel.h"
#include "nsc3/shaderfragmentmanager.h"

//------------------------------------------------------------------------------
namespace Tools
{
using namespace Util;

//------------------------------------------------------------------------------
ShaderFragmentModel::ShaderFragmentModel(QObject *parent)
	: QStandardItemModel(parent)
{
	this->SetupItems();
}

//------------------------------------------------------------------------------
ShaderFragmentModel::~ShaderFragmentModel()
{

}

//------------------------------------------------------------------------------
QStringList
ShaderFragmentModel::mimeTypes() const
{
	return QStringList() << "application/x-qstandarditemmodeldatalist" << "text/uri-list";
}

//------------------------------------------------------------------------------
void
ShaderFragmentModel::AddItem(const QString& name)
{
	QIcon theIcon = QApplication::style()->standardIcon(QStyle::SP_DriveNetIcon);

	QStandardItem* item = new QStandardItem(theIcon, name);
	item->setData(name);
	this->appendRow(item);
}

//------------------------------------------------------------------------------
void
ShaderFragmentModel::SetupItems()
{
	ShaderFragmentManager* fragManager = ShaderFragmentManager::Instance();
	n_assert(NULL != fragManager);

	const Dictionary<String, Ptr<ShaderFragment>>& fragments = fragManager->GetFragments();
	for (IndexT i = 0; i < fragments.Size(); ++i)
	{
		const Ptr<ShaderFragment>& fragment = fragments.ValueAtIndex(i);
		QString name = fragment->GetName().AsCharPtr();
		this->AddItem(name);
	}
}

}// namespace Tools
//------------------------------------------------------------------------------