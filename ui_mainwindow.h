/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSave;
    QAction *actionSave_As;
    QAction *actionOpen;
    QAction *actionSave_as_JSON;
    QAction *action_Zoom_All;
    QAction *actionZoom_In;
    QAction *actionZoom_Out;
    QAction *actionRun;
    QAction *actionSave_as_Script;
    QAction *actionLoad_Script;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QDockWidget *dockWidget_GW;
    QWidget *dockWidgetContents_4;
    QVBoxLayout *verticalLayout_3;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuRecent_Files;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidget_props;
    QWidget *dockWidgetContents_2;
    QVBoxLayout *verticalLayout_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QTreeView *treeView;
    QTableView *tableView;
    QToolBar *filetoolBar;
    QToolBar *ViewtoolBar;
    QToolBar *runtoolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(930, 703);
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        QIcon icon;
        icon.addFile(QStringLiteral("../resources/Icons/Save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave->setIcon(icon);
        actionSave_As = new QAction(MainWindow);
        actionSave_As->setObjectName(QStringLiteral("actionSave_As"));
        actionSave_As->setIcon(icon);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        QIcon icon1;
        icon1.addFile(QStringLiteral("../resources/Icons/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon1);
        actionSave_as_JSON = new QAction(MainWindow);
        actionSave_as_JSON->setObjectName(QStringLiteral("actionSave_as_JSON"));
        QIcon icon2;
        icon2.addFile(QStringLiteral("resources/Icons/Save.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_as_JSON->setIcon(icon2);
        action_Zoom_All = new QAction(MainWindow);
        action_Zoom_All->setObjectName(QStringLiteral("action_Zoom_All"));
        QIcon icon3;
        icon3.addFile(QStringLiteral("../resources/Icons/Full_screen_view.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Zoom_All->setIcon(icon3);
        actionZoom_In = new QAction(MainWindow);
        actionZoom_In->setObjectName(QStringLiteral("actionZoom_In"));
        QIcon icon4;
        icon4.addFile(QStringLiteral("../resources/Icons/zoom_in.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoom_In->setIcon(icon4);
        actionZoom_Out = new QAction(MainWindow);
        actionZoom_Out->setObjectName(QStringLiteral("actionZoom_Out"));
        QIcon icon5;
        icon5.addFile(QStringLiteral("../resources/Icons/zoom_out.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoom_Out->setIcon(icon5);
        actionRun = new QAction(MainWindow);
        actionRun->setObjectName(QStringLiteral("actionRun"));
        QIcon icon6;
        icon6.addFile(QStringLiteral("../resources/Icons/gear_run.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRun->setIcon(icon6);
        actionSave_as_Script = new QAction(MainWindow);
        actionSave_as_Script->setObjectName(QStringLiteral("actionSave_as_Script"));
        actionLoad_Script = new QAction(MainWindow);
        actionLoad_Script->setObjectName(QStringLiteral("actionLoad_Script"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        dockWidget_GW = new QDockWidget(centralWidget);
        dockWidget_GW->setObjectName(QStringLiteral("dockWidget_GW"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(dockWidget_GW->sizePolicy().hasHeightForWidth());
        dockWidget_GW->setSizePolicy(sizePolicy);
        dockWidget_GW->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockWidgetContents_4 = new QWidget();
        dockWidgetContents_4->setObjectName(QStringLiteral("dockWidgetContents_4"));
        verticalLayout_3 = new QVBoxLayout(dockWidgetContents_4);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        dockWidget_GW->setWidget(dockWidgetContents_4);

        horizontalLayout->addWidget(dockWidget_GW);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 930, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuRecent_Files = new QMenu(menuFile);
        menuRecent_Files->setObjectName(QStringLiteral("menuRecent_Files"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::LeftToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        dockWidget_props = new QDockWidget(MainWindow);
        dockWidget_props->setObjectName(QStringLiteral("dockWidget_props"));
        dockWidget_props->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QStringLiteral("dockWidgetContents_2"));
        verticalLayout_2 = new QVBoxLayout(dockWidgetContents_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        frame = new QFrame(dockWidgetContents_2);
        frame->setObjectName(QStringLiteral("frame"));
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setMinimumSize(QSize(0, 0));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        treeView = new QTreeView(frame);
        treeView->setObjectName(QStringLiteral("treeView"));

        verticalLayout->addWidget(treeView);

        tableView = new QTableView(frame);
        tableView->setObjectName(QStringLiteral("tableView"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(20);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tableView->sizePolicy().hasHeightForWidth());
        tableView->setSizePolicy(sizePolicy1);
        tableView->setMinimumSize(QSize(0, 0));

        verticalLayout->addWidget(tableView);


        verticalLayout_2->addWidget(frame);

        dockWidget_props->setWidget(dockWidgetContents_2);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget_props);
        filetoolBar = new QToolBar(MainWindow);
        filetoolBar->setObjectName(QStringLiteral("filetoolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, filetoolBar);
        ViewtoolBar = new QToolBar(MainWindow);
        ViewtoolBar->setObjectName(QStringLiteral("ViewtoolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, ViewtoolBar);
        runtoolBar = new QToolBar(MainWindow);
        runtoolBar->setObjectName(QStringLiteral("runtoolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, runtoolBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_As);
        menuFile->addAction(actionSave_as_Script);
        menuFile->addAction(actionLoad_Script);
        menuFile->addSeparator();
        menuFile->addAction(menuRecent_Files->menuAction());
        menuRecent_Files->addSeparator();
        mainToolBar->addSeparator();
        filetoolBar->addAction(actionOpen);
        filetoolBar->addAction(actionSave);
        ViewtoolBar->addAction(action_Zoom_All);
        ViewtoolBar->addAction(actionZoom_In);
        ViewtoolBar->addAction(actionZoom_Out);
        runtoolBar->addAction(actionRun);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Aquifolium", nullptr));
        actionSave->setText(QApplication::translate("MainWindow", "Save ...", nullptr));
#ifndef QT_NO_SHORTCUT
        actionSave->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_NO_SHORTCUT
        actionSave_As->setText(QApplication::translate("MainWindow", "Save As ...", nullptr));
        actionOpen->setText(QApplication::translate("MainWindow", "Open ...", nullptr));
#ifndef QT_NO_SHORTCUT
        actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_NO_SHORTCUT
        actionSave_as_JSON->setText(QApplication::translate("MainWindow", "Save as JSON ...", nullptr));
        action_Zoom_All->setText(QApplication::translate("MainWindow", "Zoom All", nullptr));
#ifndef QT_NO_TOOLTIP
        action_Zoom_All->setToolTip(QApplication::translate("MainWindow", "Zoom All", nullptr));
#endif // QT_NO_TOOLTIP
        actionZoom_In->setText(QApplication::translate("MainWindow", "Zoom In", nullptr));
        actionZoom_Out->setText(QApplication::translate("MainWindow", "Zoom Out", nullptr));
#ifndef QT_NO_TOOLTIP
        actionZoom_Out->setToolTip(QApplication::translate("MainWindow", "Zoom Out", nullptr));
#endif // QT_NO_TOOLTIP
        actionRun->setText(QApplication::translate("MainWindow", "Run", nullptr));
#ifndef QT_NO_TOOLTIP
        actionRun->setToolTip(QApplication::translate("MainWindow", "Run Model", nullptr));
#endif // QT_NO_TOOLTIP
        actionSave_as_Script->setText(QApplication::translate("MainWindow", "Save as Script", nullptr));
        actionLoad_Script->setText(QApplication::translate("MainWindow", "Load Script", nullptr));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", nullptr));
        menuRecent_Files->setTitle(QApplication::translate("MainWindow", "Recent Files", nullptr));
        filetoolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", nullptr));
        ViewtoolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", nullptr));
        runtoolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
