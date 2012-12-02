/* Copyright (c) 2012, STANISLAW ADASZEWSKI
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of STANISLAW ADASZEWSKI nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL STANISLAW ADASZEWSKI BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#include "stdneb.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "materialeditorapplication.h"
#include "models/shaderfragmentmodel.h"
#include "models/shadersamplermodel.h"

#include "qnodeeditor/qneblock.h"
#include "qnodeeditor/qnodeseditor.h"
#include "qnodeeditor/qneport.h"

#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	this->ui->setupUi(this);

	QGraphicsScene *s = new QGraphicsScene();
	this->ui->graphicsView->setScene(s);
	this->ui->graphicsView->setRenderHint(QPainter::Antialiasing);
	// this->ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

	//QNEBlock *b = new QNEBlock(0, s);
	//b->addPort("test", 0, QNEPort::NamePort);
	//b->addPort("TestBlock", 0, QNEPort::TypePort);
	//b->addInputPort("in1");
	//b->addInputPort("in2");
	//b->addInputPort("in3");
	//b->addOutputPort("out1");
	//b->addOutputPort("out2");
	//b->addOutputPort("out3");

	//b = b->clone();
	//b->setPos(150, 0);

	//b = b->clone();
	//b->setPos(150, 150);

	this->nodesEditor = new QNodesEditor(this);
	this->nodesEditor->install(s);

	connect(this->ui->action_New, SIGNAL(triggered()), this, SLOT(OnActionNew()));
	connect(this->ui->action_Load, SIGNAL(triggered()), this, SLOT(OnActionLoad()));
	connect(this->ui->action_Save, SIGNAL(triggered()), this, SLOT(OnActionSave()));
	connect(this->ui->action_Compile, SIGNAL(triggered()), this, SLOT(OnActionCompile()));
	connect(this->ui->action_Exit, SIGNAL(triggered()),  qApp, SLOT(quit()));

	this->ui->toolBar->addAction("Auto layout", nodesEditor, SLOT(autoLayout()));
}

MainWindow::~MainWindow()
{
	delete this->ui;
}

//------------------------------------------------------------------------------
void
MainWindow::OnActionNew()
{
	Tools::MaterialEditorApplication::Instance()->New();
}

//------------------------------------------------------------------------------
void
MainWindow::OnActionLoad()
{
	IO::URI uri = "home:work/shaderlib/shaders";
	QString path = QString::fromLocal8Bit(uri.LocalPath().AsCharPtr());
	QString fileName = QFileDialog::getOpenFileName(this, QObject::tr("Load shader xml"),
	                   path, "Shaders (*.xml)");
	Tools::MaterialEditorApplication::Instance()->Load(fileName.toLocal8Bit().data());
}

//------------------------------------------------------------------------------
void
MainWindow::OnActionSave()
{
	IO::URI uri = "home:work/shaderlib/shaders";
	QString path = QString::fromLocal8Bit(uri.LocalPath().AsCharPtr());
	QString fileName = QFileDialog::getSaveFileName(this, QObject::tr("Save shader xml"),
	                   path, "Shaders (*.xml)");
	Tools::MaterialEditorApplication::Instance()->Save(fileName.toLocal8Bit().data());
}

//------------------------------------------------------------------------------
void
MainWindow::OnActionCompile()
{
	Tools::MaterialEditorApplication::Instance()->Compile();
}

//------------------------------------------------------------------------------
void
MainWindow::SetupModels()
{
	this->fragmentModel = new Tools::ShaderFragmentModel(this);
	this->ui->listViewFragments->setModel(this->fragmentModel);

	this->samplerModel = new Tools::ShaderSamplerModel(this);
	this->ui->listViewSamplers->setModel(this->samplerModel);
}

//------------------------------------------------------------------------------
QGraphicsScene*
MainWindow::GetScene() const
{
	return this->ui->graphicsView->scene();
}
