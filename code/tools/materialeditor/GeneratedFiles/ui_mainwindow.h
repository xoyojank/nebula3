/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Wed Nov 28 00:29:32 2012
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDockWidget>
#include <QtGui/QGraphicsView>
#include <QtGui/QHeaderView>
#include <QtGui/QListView>
#include <QtGui/QMainWindow>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_Save;
    QAction *action_Load;
    QAction *action_Exit;
    QAction *action_Compile;
    QAction *action_New;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QGraphicsView *graphicsView;
    QToolBar *toolBar;
    QDockWidget *dockWidgetFragments;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_2;
    QListView *listViewFragments;
    QDockWidget *dockWidgetSamplers;
    QWidget *dockWidgetContents_2;
    QVBoxLayout *verticalLayout_3;
    QListView *listViewSamplers;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(799, 598);
        action_Save = new QAction(MainWindow);
        action_Save->setObjectName(QString::fromUtf8("action_Save"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icons/Save.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Save->setIcon(icon);
        action_Load = new QAction(MainWindow);
        action_Load->setObjectName(QString::fromUtf8("action_Load"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icons/Load.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Load->setIcon(icon1);
        action_Exit = new QAction(MainWindow);
        action_Exit->setObjectName(QString::fromUtf8("action_Exit"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/icons/Exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Exit->setIcon(icon2);
        action_Compile = new QAction(MainWindow);
        action_Compile->setObjectName(QString::fromUtf8("action_Compile"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/icons/Compile.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Compile->setIcon(icon3);
        action_New = new QAction(MainWindow);
        action_New->setObjectName(QString::fromUtf8("action_New"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/icons/New.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_New->setIcon(icon4);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        verticalLayout->addWidget(graphicsView);

        MainWindow->setCentralWidget(centralWidget);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        dockWidgetFragments = new QDockWidget(MainWindow);
        dockWidgetFragments->setObjectName(QString::fromUtf8("dockWidgetFragments"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout_2 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        listViewFragments = new QListView(dockWidgetContents);
        listViewFragments->setObjectName(QString::fromUtf8("listViewFragments"));

        verticalLayout_2->addWidget(listViewFragments);

        dockWidgetFragments->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidgetFragments);
        dockWidgetSamplers = new QDockWidget(MainWindow);
        dockWidgetSamplers->setObjectName(QString::fromUtf8("dockWidgetSamplers"));
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        verticalLayout_3 = new QVBoxLayout(dockWidgetContents_2);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        listViewSamplers = new QListView(dockWidgetContents_2);
        listViewSamplers->setObjectName(QString::fromUtf8("listViewSamplers"));

        verticalLayout_3->addWidget(listViewSamplers);

        dockWidgetSamplers->setWidget(dockWidgetContents_2);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidgetSamplers);

        toolBar->addAction(action_New);
        toolBar->addAction(action_Load);
        toolBar->addAction(action_Save);
        toolBar->addSeparator();
        toolBar->addAction(action_Compile);
        toolBar->addSeparator();
        toolBar->addAction(action_Exit);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QString());
        action_Save->setText(QApplication::translate("MainWindow", "&Save", 0, QApplication::UnicodeUTF8));
        action_Load->setText(QApplication::translate("MainWindow", "&Load", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        action_Load->setToolTip(QApplication::translate("MainWindow", "Load", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        action_Exit->setText(QApplication::translate("MainWindow", "&Exit", 0, QApplication::UnicodeUTF8));
        action_Compile->setText(QApplication::translate("MainWindow", "&Compile", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        action_Compile->setToolTip(QApplication::translate("MainWindow", "Compile", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        action_New->setText(QApplication::translate("MainWindow", "&New", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        action_New->setToolTip(QApplication::translate("MainWindow", "New", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", 0, QApplication::UnicodeUTF8));
        dockWidgetFragments->setWindowTitle(QApplication::translate("MainWindow", "Fragments", 0, QApplication::UnicodeUTF8));
        dockWidgetSamplers->setWindowTitle(QApplication::translate("MainWindow", "Samplers", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
