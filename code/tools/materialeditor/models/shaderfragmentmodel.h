#ifndef SHADERFRAGMENTMODEL_H
#define SHADERFRAGMENTMODEL_H
//------------------------------------------------------------------------------
/**
    @class Tools::ShaderFragmentModel

	The item model of ShaderFragment for QListBox.

    (C) 2012 xoyojank
*/
#include <QtGui>
#include <QStandardItemModel>

//------------------------------------------------------------------------------
namespace Tools
{

class ShaderFragmentModel : public QStandardItemModel
{
	Q_OBJECT

public:
	ShaderFragmentModel(QObject *parent);
	~ShaderFragmentModel();

	QStringList mimeTypes() const;

private:
	void AddItem(const QString& name);
	void SetupItems();
	
};

}// namespace Tools
//------------------------------------------------------------------------------

#endif // SHADERFRAGMENTMODEL_H
