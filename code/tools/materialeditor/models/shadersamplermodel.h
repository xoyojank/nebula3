#ifndef SHADERSAMPLERMODEL_H
#define SHADERSAMPLERMODEL_H
//------------------------------------------------------------------------------
/**
    @class Tools::ShaderSamplerModel

	The item model of ShaderSampler for QListBox.

    (C) 2012 xoyojank
*/
#include <QtGui>
#include <QStandardItemModel>

//------------------------------------------------------------------------------
namespace Tools
{

class ShaderSamplerModel : public QStandardItemModel
{
	Q_OBJECT

public:
	ShaderSamplerModel(QObject *parent);
	~ShaderSamplerModel();

	QStringList mimeTypes() const;

private:
	void AddItem(const QString& name);
	void SetupItems();
	
};

}// namespace Tools
//------------------------------------------------------------------------------

#endif // SHADERSAMPLERMODEL_H
