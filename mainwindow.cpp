#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "QGraphicsRectItem"
#include "aqflmblockitem.h"
#include "logwindow.h"
#include "node.h"
#include "edge.h"
#include "delegate.h"
#include "qmessagebox.h"
#include "folders.h"

#define RECENT "recentFiles.txt"

#define cout qDebug()

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    string modelfilename = qApp->applicationDirPath().toStdString() + "/resources/power_reservoirs.qnt";
    system.GetQuanTemplate(modelfilename);
    ui->setupUi(this);
    qDebug()<<qApp->applicationDirPath();
    logWindow* logwindow = new logWindow(this);
    //diagramview = new View("Diagram",ui->dockWidgetContents_4);
    diagramview = new GraphWidget(this,"Aquifolium","",logwindow,this);
    diagramview->setObjectName(QStringLiteral("graphicsView_2"));
    diagramview->tableProp = ui->tableView;

    projModel = new TreeModel(diagramview);
    ui->treeView->setModel(projModel);

    ui->treeView->setObjectName(QStringLiteral("projectExplorer"));

    Qt::WindowFlags flags = this->windowFlags();
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

    // ***
    diagramview->setProjExp(ui->treeView);
    diagramview->treeModel = projModel;

    Delegate *mDelegate = new Delegate(this, diagramview);

    ui->tableView->setItemDelegateForColumn(1,mDelegate);
    diagramview->settableProp(ui->tableView);
    diagramview->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::AllEditTriggers
        | QAbstractItemView::SelectedClicked);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);

    ui->verticalLayout_3->addWidget(diagramview);
    for (unsigned int i=0; i<system.GetAllBlockTypes().size(); i++)
    {
        qDebug()<<QString::fromStdString(system.GetAllBlockTypes()[i]);
        QAction* action = new QAction(this);
        action->setObjectName(QString::fromStdString(system.GetAllBlockTypes()[i]));
        QIcon icon;
        icon.addFile(QString::fromStdString(qApp->applicationDirPath().toStdString()+"/resources/Icons/"+system.GetModel(system.GetAllBlockTypes()[i])->IconFileName()), QSize(), QIcon::Normal, QIcon::Off);
        action->setIcon(icon);
        ui->mainToolBar->addAction(action);
        action->setText(QString::fromStdString(system.GetAllBlockTypes()[i]));
        connect(action,SIGNAL(triggered()),this,SLOT(onaddblock()));
    }
    ui->mainToolBar->addSeparator();
    for (unsigned int i=0; i<system.GetAllLinkTypes().size(); i++)
    {
        qDebug()<<QString::fromStdString(system.GetAllLinkTypes()[i]);
        QAction* action = new QAction(this);
        action->setCheckable(true);
        action->setObjectName(QString::fromStdString(system.GetAllLinkTypes()[i]));
        QIcon icon;
        icon.addFile(QString::fromStdString(qApp->applicationDirPath().toStdString()+"/resources/Icons/"+system.GetModel(system.GetAllLinkTypes()[i])->IconFileName()), QSize(), QIcon::Normal, QIcon::Off);
        action->setIcon(icon);
        ui->mainToolBar->addAction(action);
        action->setText(QString::fromStdString(system.GetAllLinkTypes()[i]));
        connect(action,SIGNAL(triggered()),this,SLOT(onaddlink()));
    }

    connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(on_action_Save_triggered()));
    //scene = new QGraphicsScene(this);
    //diagramview->view()->setScene(scene);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onaddlink()
{
    QObject* obj = sender();
    diagramview->setconnectfeature(obj->objectName());
    foreach (QAction* action, ui->mainToolBar->actions())
    {
        if (action->objectName()!=obj->objectName())
            action->setChecked(false);
        else
            action->setChecked(true);
    }

}
void MainWindow::onaddblock()
{
    QObject* obj = sender();
    counts[obj->objectName()]=counts[obj->objectName()]+1;
    qDebug()<<"block added! " << obj->objectName();
    qDebug()<<"creating new AqflmBlockItem";
    Node* item = new Node(diagramview,obj->objectName(),obj->objectName() + QString::number(counts[obj->objectName()]),int(diagramview->scene()->width()/2), int(diagramview->scene()->height()/2));
    item->seticonfilename(qApp->applicationDirPath()+"/resources/Icons/"+QString::fromStdString(system.GetModel(obj->objectName().toStdString())->IconFileName()));

}

void MainWindow::on_action_Save_triggered()
{
    QString fileName = (!diagramview->modelFilename.isEmpty()) ? diagramview->modelFilename : QFileDialog::getSaveFileName(this,
        tr("Save ").append(applicationName), diagramview->modelPathname(),
        tr("Model (*.").append(".aqf").append(");;All Files (*)"));
    if (saveModel(fileName))
    {
        setModelFileName(fileName);
        if (fileName.right(4) != "temp")
            addToRecentFiles(fileName);
    }
}

bool MainWindow::saveModel(QString &fileName)
{
    if (fileName.isEmpty())
        return false;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return false;
        }
        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_4_5);
#ifdef Aquifolium
        out << QString("Aquifolium Data File version 1.0");
#endif
#ifdef GIFMOD
        out << QString("GIFMod Data File version 1.3"); //1.1 with defaultUnit // 1.2 Multi experiment
#endif
#ifdef GWA
        out << QString("GWA Data File version 1.2"); // with defaultUnit
#endif
        clock_t t0, t1;		t0 = clock();

        {
            t1 = clock() - t0;		float run_time = ((float)t1) / CLOCKS_PER_SEC;		QString st = " sec";		if (run_time >= 60) { run_time /= 60; st = " min"; }
            if (run_time >= 60) { run_time /= 60; st = " hr"; }		QString r = QString("%1 %2").arg(run_time).arg(st);
            getTime();
            out << diagramview->compact();

            file.flush();

            file.close();
        }


        return true;
    }
}

void MainWindow::setModelFileName(const QString &fileName)
{
    diagramview->modelFilename = fileName;
    setWindowTitle(QString("%1 - %2").arg(OnlyFilename(fileName)).arg(applicationName));
}

void MainWindow::addToRecentFiles(QString fileName, bool addToFile)
{
    bool rewriteFile = false;
    if (recentFiles.contains(fileName.toLower()) && fileName.trimmed() != "")
        if (recentFiles.indexOf(fileName.toLower()) != recentFiles.count()-1)
        {
            ui->menuRecent_Files->removeAction(ui->menuRecent_Files->actions()[recentFiles.size() - 1 - recentFiles.indexOf(fileName.toLower())]);
            recentFiles.removeOne(fileName.toLower());
            addToFile = false;
            rewriteFile = true;
        }

    if (!recentFiles.contains(fileName.toLower()) && fileName.trimmed() != "")
    {
        recentFiles.append(fileName.toLower());
        //		QAction * a = ui->menuRecent->addAction(fileName);// , this, SLOT(recentItem()));
        QAction * fileNameAction = new QAction(fileName, nullptr);
        if (ui->menuRecent_Files->actions().size())
            ui->menuRecent_Files->insertAction(ui->menuRecent_Files->actions()[0], fileNameAction);
        else
            ui->menuRecent_Files->addAction(fileNameAction);
        QObject::connect(fileNameAction, SIGNAL(triggered()), this, SLOT(on_actionRecent_triggered()));

        if (addToFile)
        {
            ofstream file(localAppFolderAddress().toStdString() + RECENT, fstream::app);
            if (file.good())
                file << fileName.toStdString() << std::endl;
            file.close();
        }
        if (rewriteFile)
            writeRecentFilesList();
    }
}

void MainWindow::writeRecentFilesList()
{
    ofstream file(localAppFolderAddress().toStdString() + RECENT);
    if (file.good())
    {
        int i = recentFiles.removeDuplicates();
        foreach (QString fileName , recentFiles)
            file << fileName.toStdString() << std::endl;
    }
    file.close();
}


