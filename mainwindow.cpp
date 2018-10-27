#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "QGraphicsRectItem"
#include "logwindow.h"
#include "node.h"
#include "edge.h"
#include "delegate.h"
#include "qmessagebox.h"
#include "folders.h"
#include <QJsonDocument>
#include "runtimeWindow.h"

#define RECENT "recentFiles.txt"


#define cout qDebug()

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    fileExtension = "aqfm";
    modelfilename = qApp->applicationDirPath().toStdString() + "/resources/power_reservoirs.qnt";
    system.GetQuanTemplate(modelfilename);
    ui->setupUi(this);
    qDebug()<<qApp->applicationDirPath();
    logWindow* logwindow = new logWindow(this);
    //diagramview = new View("Diagram",ui->dockWidgetContents_4);
    logwindow->show();
    logwindow->append("Program started");
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
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(on_action_Open_triggered()));
    connect(ui->actionSave_as_JSON, SIGNAL(triggered()),this,SLOT(on_actionSave_As_JSON_triggered()));
    connect(ui->actionSave_As, SIGNAL(triggered()),this,SLOT(on_actionSave_As_triggered()));
    connect(ui->action_Zoom_All, SIGNAL(triggered()),this,SLOT(on_actionZoom_All_triggered()));
    connect(ui->actionRun, SIGNAL(triggered()),this, SLOT(on_actionRun_Model_triggered()));
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
        tr("Model (*.").append(fileExtension).append(");;All Files (*)"));
    if (saveModel(fileName))
    {
        setModelFileName(fileName);
        if (fileName.right(4) != "temp")
            addToRecentFiles(fileName);
    }
}

void MainWindow::on_actionSave_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save ").append(applicationName), diagramview->modelPathname(),
        tr("Model (*.").append(fileExtension).append(");;All Files (*)"));
    Entity *e = diagramview->entityByName("Project settings (1)");

    diagramview->Entities.removeOne(e);
    if (fileName.right(fileExtension.size()+1)!="."+fileExtension)
        fileName += "."+fileExtension;
    saveModel(fileName);

    if (saveModel(fileName))
    {
        setModelFileName(fileName);
        if (fileName.right(4) != "temp")
            addToRecentFiles(fileName);
    }
}

void MainWindow::on_actionSave_As_JSON_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save ").append(applicationName), diagramview->modelPathname(),
        tr("JSON (*.").append("json").append(");;All Files (*)"));
    Entity *e = diagramview->entityByName("Project settings (1)");

    diagramview->Entities.removeOne(e);
    if (fileName.right(fileExtension.size()+1)!=".json")
        fileName += ".json";

    if (saveModelJson(fileName)) {
        setModelFileName(fileName);
        if (fileName.right(4) != "temp")
            addToRecentFiles(fileName);
    }
}

bool MainWindow::saveModelJson(QString &fileName)
{
    if (fileName.isEmpty())
        return false;
    else {
        QFile saveFile(fileName);
        if (!saveFile.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                saveFile.errorString());
            return false;
        }

        QJsonObject json;
        diagramview->compact(json);
        QJsonDocument saveDoc(json);
        saveFile.write(saveDoc.toJson());
        saveFile.flush();
        saveFile.close();

    }

    return true;

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
            t1 = clock() - t0;		float run_time = (float(t1)) / CLOCKS_PER_SEC;		QString st = " sec";		if (run_time >= 60) { run_time /= 60; st = " min"; }
            if (run_time >= 60) { run_time /= 60; st = " hr"; }		QString r = QString("%1 %2").arg(double(run_time)).arg(st);
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
        recentFiles.removeDuplicates();
        foreach (QString fileName , recentFiles)
            file << fileName.toStdString() << std::endl;
    }
    file.close();
}

void MainWindow::on_action_Open_triggered()
{
    //open
    if (diagramview->hasChanged())
        if (QMessageBox::question(this, tr("Open Model"),
            "Your model has not been saved.\nAll changes in the model will be lost.\nAre you sure?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
            return;
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open ").append(applicationName), diagramview->modelPathname(),
        tr("Model (*.").append(fileExtension).append(");;All Files (*)"));
    diagramview->clear();
    if (fileName == "") return;
    if (loadModel(fileName))
    {
        //diagramview->modelSet->load(diagramview, rtw); !Attention
        on_actionZoom_All_triggered();
        diagramview->updateNodeCoordinates();
        addToRecentFiles(fileName);
    }
    else
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::information(this, "File Corrupted!", "The model file is corrupted.",
            QMessageBox::Ok);
    }


}
bool MainWindow::loadModel(QString modelfilename)
{
    if (modelfilename.isEmpty())
        return false;
    else {
        QFile file(modelfilename);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return false;
        }
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_4_5);
        QString fileHeader;
        in >> fileHeader;
        QList <QMap<QString, QVariant>> dataMap;

        in >> dataMap;
        file.close();

        QString previousModelFilename = diagramview->modelFilename;
        setModelFileName(modelfilename);


        diagramview->trackingUndo = false;
        diagramview->clear();
        diagramview->unCompact(dataMap);

    }

    foreach (Node *n , diagramview->Nodes())
    {
        n->setProp("x", n->x());
        n->setProp("y", n->y());
    }

    //	updateInterface(NavigationMode);
    diagramview->changedState = false;
    diagramview->trackingUndo = true;
    return true;
}

void MainWindow::on_actionZoom_In_triggered()
{
    //Zoom In
    //mainGraphWidget->zoomIn();

    diagramview->scaleView(1.25);
}

void MainWindow::on_actionZoom_Out_triggered()
{
    diagramview->scaleView(1 / 1.25);
}

void MainWindow::on_actionZoom_All_triggered()
{
    QRectF newRect = diagramview->MainGraphicsScene->itemsBoundingRect();
    float width = float(newRect.width());
    float height = float(newRect.height());
    float scale = float(1.1);
    newRect.setLeft(newRect.left() - float(scale - 1) / 2 * float(width));
    newRect.setTop(newRect.top() - (scale - 1) / 2 * height);
    newRect.setWidth(qreal(width * scale));
    newRect.setHeight(qreal(height * scale));

    diagramview->fitInView(newRect,Qt::KeepAspectRatio);
}


void MainWindow::on_actionE_xit_triggered()
{
    this->close();
}

void MainWindow::on_actionRun_Model_triggered()
{
    diagramview->tableProp->setModel(nullptr);
    if (!diagramview->allowRun)
    {
        statusBar()->showMessage("Unable to run Model.");
        diagramview->log(QString("Unable to run Model, the value of [%1] in property dialog has not been confirmed.").arg(diagramview->allowRunVariableName));
        diagramview->allowRun = true;
        return;
    }

    if (!diagramview->Nodes().size())
    {
        statusBar()->showMessage("Unable to run Model.");
        diagramview->log("The model should have at least one block to run.");
        return;
    }
#ifdef Aquifolium
    QString fileName = diagramview->modelFilename;
    diagramview->modelFilename.replace(QString(".").append(fileExtension), ".temp");
    on_action_Save_triggered();
    if (fileName != "") setModelFileName(fileName);
    QString statusBarText = statusBar()->currentMessage();
    diagramview->log("Checking for Errors.");
    statusBar()->showMessage("Checking for Errors.");
    setCursor(Qt::WaitCursor);
    diagramview->logW->writetotempfile();
    QStringList result = diagramview->variableValuesHasError();
    QString logMsg;
    diagramview->log("-------------------------------------------");
    logMsg.append(QString("====   %1 Errors, %2 Warnings   ====").arg(result[0]).arg(result[1]));
    diagramview->log(logMsg);
    if (result[0].toInt()>0)
    {
        setCursor(Qt::ArrowCursor);
        statusBar()->showMessage("Unable to run Model.");
        diagramview->log("Faild to run the Model.");
        diagramview->deselectAll();
        return;
    }
    diagramview->log("Assembling model configuration.");
    statusBar()->showMessage("Assembling model configuration.");
    diagramview->logW->writetotempfile();
    QCoreApplication::processEvents();
    rtw = new runtimeWindow(diagramview);
    diagramview->deleteSolutionResults();
    diagramview->model = new System(diagramview, rtw,modelfilename);
    rtw->show();
    diagramview->log("Running Simulation.");
    statusBar()->showMessage("Running Simulation.");
    forwardRun(diagramview->model, rtw);
    setCursor(Qt::ArrowCursor);
    statusBar()->showMessage("Done.");
#endif
}


void MainWindow::forwardRun(System *model, runtimeWindow* progress)
{
    clock_t t0, t1;
    t0 = clock();
    ofstream runtime_file;
    //string a = model->FI.pathname;
    modelpathname = diagramview->modelFilename.section("/",0,-2);
    qDebug()<<modelPathname();
    runtime_file.open(modelPathname().toStdString() + "Runing_times.txt");
    model->Solve("Storage");
    t1 = clock() - t0;
    float run_time = (float(t1)) / CLOCKS_PER_SEC;
    string st = " sec";
    if (run_time >= 60) { run_time /= 60; st = " min"; }
    if (run_time >= 60) { run_time /= 60; st = " hr"; }
    runtime_file << "Run Time :  " << run_time << st << std::endl;
    runtime_file << "Epoch count: " << model->EpochCount() << std::endl;

    model->GetOutputs().writetofile(modelPathname().toStdString()+"Results.txt", true);

    //system.Solution_dt.writetofile(system.outputpathname()+"dt.txt");
    runtime_file.close();

//	mainGraphWidget->results->projected = system->projected;
//	model->ANS_obs = model->modeled;
//	mainGraphWidget->results->ANS_obs = model->ANS_obs;
    //mainGraphWidget->model = &model->Medium[0];

    diagramview->hasResults = true;

}
