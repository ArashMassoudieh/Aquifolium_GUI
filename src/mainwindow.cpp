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
#include "csvEditor.h"
#include "helpWindow.h"
#include "qinputdialog.h"
#include "results.h"
#include "entity.h"
#include "BTC.h"


#define RECENT "recentFiles.txt"


#define cout qDebug()

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    fileExtension = "aqfm";
#ifndef Win_Version
    modelfilename = qApp->applicationDirPath().toStdString() + "/resources/power_reservoirs_rules_source.json";
    entitiesfilename = qApp->applicationDirPath().toStdString() + "/resources/entities.json";
#else
	modelfilename = qApp->applicationDirPath().toStdString() + "/resources/power_reservoirs_rules_source.json";
	entitiesfilename = qApp->applicationDirPath().toStdString() + "/resources/entities.json";
#endif // !Win_Version

	

    system.GetQuanTemplate(modelfilename);
    ui->setupUi(this);
    qDebug()<<qApp->applicationDirPath();
    logWindow* logwindow = new logWindow(this);
    //diagramview = new View("Diagram",ui->dockWidgetContents_4);
    logwindow->show();
    logwindow->append("Program started");
    diagramview = new GraphWidget(this,"Aquifolium","",logwindow,this);
    ReadEntitiesJson();
	
	diagramview->mList->AppendEntities(&diagramview->jsondocentities);
    diagramview->setObjectName(QStringLiteral("DiagramView"));
    diagramview->tableProp = ui->tableView;

    projModel = new TreeModel(diagramview);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeView->setModel(projModel);
    ui->treeView->setObjectName(QStringLiteral("projectExplorer"));
    diagramview->setProjExp(ui->treeView);
    diagramview->treeModel = projModel;
    projModel->Populate(diagramview);
    Qt::WindowFlags flags = this->windowFlags();
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);

    // ***


    Delegate *mDelegate = new Delegate(this, diagramview);

    ui->tableView->setItemDelegateForColumn(1,mDelegate);
    diagramview->settableProp(ui->tableView);
    diagramview->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::AllEditTriggers
        | QAbstractItemView::SelectedClicked);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);

    ui->verticalLayout_3->addWidget(diagramview);
    
	BuildObjectsToolBar();

    connect(ui->actionSave,SIGNAL(triggered()),this,SLOT(on_action_Save_triggered()));
    connect(ui->actionOpen,SIGNAL(triggered()),this,SLOT(on_action_Open_triggered()));
    connect(ui->actionSave_as_JSON, SIGNAL(triggered()),this,SLOT(on_actionSave_As_JSON_triggered()));
    connect(ui->actionSave_As, SIGNAL(triggered()),this,SLOT(on_actionSave_As_triggered()),Qt::UniqueConnection);
	connect(ui->actionSave_as_Script, SIGNAL(triggered()), this, SLOT(on_actionSave_As_Script_triggered()), Qt::UniqueConnection);
    connect(ui->action_Zoom_All, SIGNAL(triggered()),this,SLOT(on_actionZoom_All_triggered()));
    connect(ui->actionRun, SIGNAL(triggered()),this, SLOT(on_actionRun_Model_triggered()));
    connect(ui->treeView,SIGNAL(clicked(const QModelIndex&)),this, SLOT(on_projectExplorer_clicked(const QModelIndex&)));
    connect(ui->treeView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(on_projectExplorer_customContextMenuRequested(const QPoint &)));
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tableView, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(tablePropShowContextMenu(const QPoint&)));

    connect(diagramview, SIGNAL(changed()),
        this, SLOT(gwidgetChanged()));

    connect(diagramview, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(on_projectExplorer_customContextMenuRequested(const QPoint &)));

}

bool MainWindow::BuildObjectsToolBar()
{
	for (unsigned int i = 0; i < system.GetAllBlockTypes().size(); i++)
	{
		qDebug() << QString::fromStdString(system.GetAllBlockTypes()[i]);
		QAction* action = new QAction(this);
		action->setObjectName(QString::fromStdString(system.GetAllBlockTypes()[i]));
		QIcon icon;
		icon.addFile(QString::fromStdString(qApp->applicationDirPath().toStdString() + "/resources/Icons/" + system.GetModel(system.GetAllBlockTypes()[i])->IconFileName()), QSize(), QIcon::Normal, QIcon::Off);
		action->setIcon(icon);
		ui->mainToolBar->addAction(action);
		action->setText(QString::fromStdString(system.GetAllBlockTypes()[i]));
		connect(action, SIGNAL(triggered()), this, SLOT(onaddblock()));
	}
	ui->mainToolBar->addSeparator();
	for (unsigned int i = 0; i < system.GetAllLinkTypes().size(); i++)
	{
		qDebug() << QString::fromStdString(system.GetAllLinkTypes()[i]);
		QAction* action = new QAction(this);
		action->setCheckable(true);
		action->setObjectName(QString::fromStdString(system.GetAllLinkTypes()[i]));
		QIcon icon;
		icon.addFile(QString::fromStdString(qApp->applicationDirPath().toStdString() + "/resources/Icons/" + system.GetModel(system.GetAllLinkTypes()[i])->IconFileName()), QSize(), QIcon::Normal, QIcon::Off);
		action->setIcon(icon);
		ui->mainToolBar->addAction(action);
		action->setText(QString::fromStdString(system.GetAllLinkTypes()[i]));
		connect(action, SIGNAL(triggered()), this, SLOT(onaddlink()));
	}
	ui->mainToolBar->addSeparator();
	for (unsigned int j = 0; j < system.QGetAllCategoryTypes().size(); j++)
	{
		string typecategory = system.QGetAllCategoryTypes()[j].toStdString();
		if (typecategory!="Blocks" && typecategory !="Connectors")
			for (unsigned int i = 0; i < system.GetAllTypesOf(typecategory).size(); i++)
			{
				string type = system.GetAllTypesOf(typecategory)[i];
				QAction* action = new QAction(this);
				action->setCheckable(true);
				action->setObjectName(QString::fromStdString(type));
				QIcon icon;
				icon.addFile(QString::fromStdString(qApp->applicationDirPath().toStdString() + "/resources/Icons/" + system.GetModel(type)->IconFileName()), QSize(), QIcon::Normal, QIcon::Off);
				action->setIcon(icon);
				ui->mainToolBar->addAction(action);
				action->setText(QString::fromStdString(type));
				connect(action, SIGNAL(triggered()), this, SLOT(onaddentity()));
			}
		ui->mainToolBar->addSeparator();
	}

	return true; 
}

bool MainWindow::ReadEntitiesJson() {
    QFile loadFile(QString::fromStdString(entitiesfilename));

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qDebug() << QString("Couldn't open ") << QString::fromStdString(entitiesfilename) ;
        return false;
    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    diagramview->jsondocentities = loadDoc;

    QJsonObject jsonobj = loadDoc.object();
    return true;
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
    Node* item = new Node(diagramview,obj->objectName(),obj->objectName() + QString::number(counts[obj->objectName()]),int(diagramview->scene()->width()/2), int(diagramview->scene()->height()/2));
    item->seticonfilename(qApp->applicationDirPath()+"/resources/Icons/"+QString::fromStdString(system.GetModel(obj->objectName().toStdString())->IconFileName()));

}

void MainWindow::onaddsource()
{
	QObject* obj = sender();
	counts[obj->objectName()] = counts[obj->objectName()] + 1;
	qDebug() << "source added! " << obj->objectName();
	Entity* item = new Entity(obj->objectName(), obj->objectName() + QString::number(counts[obj->objectName()]), diagramview,"Sources");

}

void MainWindow::onaddentity()
{
	QObject* obj = sender();
	counts[obj->objectName()] = counts[obj->objectName()] + 1;
	qDebug() << "entity added! " << obj->objectName();
	Entity* item = new Entity(obj->objectName(), obj->objectName() + QString::number(counts[obj->objectName()]), diagramview, QString::fromStdString(system.GetMetaModel()->GetItem(obj->objectName().toStdString())->CategoryType()));

}

void MainWindow::on_action_Save_triggered()
{
    QString fileName = (!diagramview->modelFilename.isEmpty()) ? diagramview->modelFilename : QFileDialog::getSaveFileName(this,
        tr("Save ").append(applicationName), diagramview->modelPathname(),
        tr("Model (*.").append(fileExtension).append(");;All Files (*);; JSON (*.json)"));
    if (fileName.right(4).toLower()!="json")
    {   if (saveModel(fileName))
        {
            setModelFileName(fileName);
            if (fileName.right(4) != "temp")
                addToRecentFiles(fileName);
        }
    }
    else {
        if (saveModelJson(fileName))
        {
            setModelFileName(fileName);
            addToRecentFiles(fileName);
        }
    }
}

void MainWindow::on_actionSave_As_Script_triggered()
{
	QFileDialog filedlg(this, "Save As ...");
	QString selectedfilter;
	QString fileName = filedlg.getSaveFileName(this,
		tr("Save ").append(""), diagramview->modelPathname(),
		tr("Script (*.scr);;All Files (*)"), &selectedfilter);
	qDebug() << filedlg.extension();
	qDebug() << selectedfilter;
	saveModel_to_script(fileName);
}

void MainWindow::on_actionSave_As_triggered()
{
    QFileDialog filedlg(this,"Save As ...");
    QString selectedfilter;
    QString fileName = filedlg.getSaveFileName(this,
        tr("Save ").append(applicationName), diagramview->modelPathname(),
        tr("Model (*.").append(fileExtension).append(");;JSON (*.json);;All Files (*)"),&selectedfilter);
    qDebug() << filedlg.extension();
    qDebug() << selectedfilter;
    Entity *e = diagramview->entityByName("Project settings (1)");

    diagramview->Entities.removeOne(e);
    if (selectedfilter == tr("Model (*.").append(fileExtension))
    {   if (fileName.right(fileExtension.size()+1)!="."+fileExtension)
            fileName += "."+fileExtension;
    }
    if (selectedfilter == "JSON (*.json)")
    {   if (fileName.right(5)!=".json")
            fileName += ".json";
    }
    saveModel(fileName);

    if (fileName.right(4).toLower()!="json")
    {   if (saveModel(fileName))
        {
            setModelFileName(fileName);
            if (fileName.right(4) != "temp")
                addToRecentFiles(fileName);
        }
    }
    else {
        if (saveModelJson(fileName))
        {
            setModelFileName(fileName);
            addToRecentFiles(fileName);
        }
    }
}

void MainWindow::on_actionSave_As_JSON_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save ").append(applicationName), diagramview->modelPathname(),
        tr("JSON (*.").append("json").append(");;All Files (*)"));

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


bool MainWindow::saveModel_to_script(QString& fileName)
{
	QFile fOut(fileName);
	if (!fOut.open(QIODevice::WriteOnly)) {
		QMessageBox::information(this, tr("Unable to open file"),
			fOut.errorString());
		return false;
	}
	QTextStream out(&fOut);
	QStringList str; 
	for (Node* n : diagramview->Nodes())
	{
		str<<n->toCommand();
	}
    for (Edge* e : diagramview->Edges())
    {
        str<<e->toCommand();
    }
	for (QStringList::Iterator it = str.begin(); it != str.end(); ++it)
		out << *it << "\n";
	fOut.flush();
	fOut.close(); 
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
        tr("Model (*.").append(fileExtension).append(");;JSON (*.json);;All Files (*)"));
    diagramview->clear();
    if (fileName == "") return;
    if (fileName.right(4).toLower()!="json")
    {   if (loadModel(fileName))
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
    else {
        if (loadModelJSON(fileName))
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

bool MainWindow::loadModelJSON(QString modelfilename)
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
        QByteArray saveData = file.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
        QJsonObject jsonobj = loadDoc.object();

        file.close();

        QString previousModelFilename = diagramview->modelFilename;
        setModelFileName(modelfilename);


        diagramview->trackingUndo = false;
        diagramview->clear();
        diagramview->unCompact(jsonobj);

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
    model->SetLogWindow(diagramview->logW);
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

void MainWindow::on_projectExplorer_clicked(const QModelIndex& index)
{
    //QModelIndex index = ui->treeView->currentIndex();
    qDebug()<<index << index.data().toString();
    if (index.data(Role::TreeItemType) == TreeItem::Type::NodeItem ||
        index.data(Role::TreeItemType) == TreeItem::Type::EdgeItem ||
        index.data(Role::TreeItemType) == TreeItem::Type::EntityItem ||
        index.data(Role::TreeItemType) == TreeItem::Type::Item)
    {
        QString type = index.data(Role::TreeParentItemType).toString();
        diagramview->select(index.data().toString(), type);
    }
}

void MainWindow::tablePropShowContextMenu(const QPoint&pos)
{
    QModelIndex i1 = ui->tableView->indexAt(pos);
    int row = i1.row();
    QModelIndex i2 = i1.sibling(row, 1);

    if (i1.column() == 0)
    {
        QMenu *menu = new QMenu;
        int code = i1.data(DescriptionCodeRole).toInt();
        QString variableName = i1.data(VariableNameRole).toString();
        showHelp(code, variableName);
        menu->addAction("Help", this, SLOT(showHelp()));
        menu->addSeparator();
        qDebug()<<i1.data(VariableTypeRole).toString().toLower();
        qDebug()<<i2.data(VariableTypeRole).toString().toLower();
        if (i2.data(VariableTypeRole).toString().toLower().contains("getnumber"))
        {
            double initial = i2.data(Qt::ToolTipRole).toDouble();
            getNumber(initial);
            menu->addAction("Input number", this, SLOT(getNumber()));
            addParameterIndex(i1); // tableProp->indexAt(pos));
            menu->addSeparator();
        }
        QMenu *estimatesMenu = new QMenu("Parameters");
        menu->addMenu(estimatesMenu);
        estimatesMenu->setEnabled(false);
        if (i2.data(VariableTypeRole).toString().toLower().contains("estimate"))
        {
            foreach (QString item , diagramview->EntityNames("Parameter"))
                estimatesMenu->addAction(QString("%1").arg(item));// , this, SLOT(addParameter()));
            addParameterIndex(i1); // tableProp->indexAt(pos));
            connect(estimatesMenu, SIGNAL(triggered(QAction*)), this, SLOT(addParameter(QAction*)));
            estimatesMenu->setEnabled(true);
        }
        QMenu *controlsMenu = new QMenu("Controls");
        menu->addMenu(controlsMenu);
        controlsMenu->setEnabled(false);
        if (i2.data(VariableTypeRole).toString().toLower().contains("control"))
        {
            foreach (QString item , diagramview->EntityNames("Controller"))
                controlsMenu->addAction(QString("%1").arg(item));// , this, SLOT(addParameter()));
            addParameterIndex(i1); // tableProp->indexAt(pos));
            connect(controlsMenu, SIGNAL(triggered(QAction*)), this, SLOT(addParameter(QAction*)));
            controlsMenu->setEnabled(true);
        }

        menu->exec(ui->tableView->mapToGlobal(pos));
    }
    if (i1.column() == 1 && i1.data(TypeRole).toString().toLower().contains("time series"))
    {
        QStringList graphNames;
        QString file = i2.data().toString();
        QString fullfile = i2.data(Qt::ToolTipRole).toString();
        fullfile = fullFilename(fullfile, diagramview->modelPathname());
        //QString fullfile = fullFilename(QString("./%1").arg(file), modelPathname());
        CBTCSet data;
#ifdef GIFMOD
        if (i1.data(TypeRole).toString().toLower().contains("precipitation"))
            data = CPrecipitation(fullfile.toStdString()).getflow(1,1.0/24.0/4.0);
        else
#endif
            data = CBTCSet(fullfile.toStdString(), 1);

        foreach (string name , data.names)
        {
            if (!graphNames.contains(QString::fromStdString(name)))
                graphNames.append(QString::fromStdString(name));
            else diagramview->warning(QString("Duplicate header %1 in time series, file %1").arg(QString::fromStdString(name)).arg(file));
        }
        QMenu *menu = new QMenu;
        menu->addAction("Edit time series");
        if (graphNames.count())
        {
            bool convertXtoTime = true;
            if (i1.data(TypeRole).toString().toLower().contains("age"))
                convertXtoTime = false;
            plotTimeSeries(nullptr, CBTC(), "", true, true);
            foreach (QString subTitle , graphNames)
            {
                QAction* action = menu->addAction(subTitle);
                plotTimeSeries(action, data[subTitle.toStdString()], subTitle, convertXtoTime);
            }
            connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(plotTimeSeries(QAction*)));
        }
        QAction *action = menu->exec(ui->tableView->mapToGlobal(pos));
        bool precipitation = (i1.data(TypeRole).toString().toLower().contains("precipitation")) ? true : false;

        if (action)
            if (action->text() == "Edit time series")
                csvEditor *editor = new csvEditor(this, precipitation, fullfile, fullfile, ui->tableView, ui->tableView->indexAt(pos));

    }
}

void MainWindow::showHelp(int code, QString variableName)
{
    static int helpCode;
    static QString helpVariableName;

    if (code != -1)
    {
        helpCode = code;
        helpVariableName = variableName;
        return;
    }

    diagramview->help->showHelp(helpCode, helpVariableName.toStdString());
}

void MainWindow::getNumber(double initial)
{
    static double initialValue = 1;
    if (initial != -1)
    {
        initialValue = initial;
        return;
    }

    double value = QInputDialog::getDouble(this, "Input Dialog Box", "Enter number:", initialValue, 0, 2147483647, 4);// item->text();

    ui->tableView->model()->setData(addParameterIndex(), value);
}


void MainWindow::plotTimeSeries(QAction* action, CBTC data, QString name, bool convertXtoTime, bool reset)
{
    struct graph
    {
        QAction* action;
        CBTC data;
        QString name;
        bool convertXtoTime;
    };
    static vector<graph>graphs;

    if (reset){
        graphs.clear();
        return;
    }

    if (data.n)
    {
        graph g;
        g.action = action;
        g.data = data;
        g.name = name;
        g.convertXtoTime = convertXtoTime;
        graphs.push_back(g);
    }
    else
    {
        for (unsigned int i = 0; i < graphs.size(); i++)
            if (action == graphs[i].action)
            {
                plotWindow *plot = new plotWindow(diagramview);
                plotformat format;
                format.xAxisTimeFormat = graphs[i].convertXtoTime;
                plot->addScatterPlot(graphs[i].data, graphs[i].name, format);
                plot->show();
                return;
            }
    }
}

QModelIndex MainWindow::addParameterIndex(const QModelIndex &index)
{
    static QModelIndex parameterIndex;
    if (index != QModelIndex()) parameterIndex = index;
    return parameterIndex;
}

void MainWindow::addParameter(QAction* item)
{
    QString parameter = item->text();
    ui->tableView->model()->setData(addParameterIndex(), parameter, setParamRole);
}

void MainWindow::on_projectExplorer_customContextMenuRequested(const QPoint &pos)
{
    QMenu *menu = new QMenu;
    if (ui->treeView->indexAt(pos).data(Role::TreeItemType) == TreeItem::Type::Branch)
    {
        //QMenu *menu = new QMenu;
        TreeModel *model = projModel;
        TreeItem *item = model->itemFromIndex(ui->treeView->indexAt(pos));
        QString type = projModel->singularform(ui->treeView->indexAt(pos).data().toString());
        QModelIndex index = ui->treeView->indexAt(pos);
        addProjectExplorerTreeItem(model, type, index);
        if (type == "Parameter" && diagramview->results)
        {
            if (diagramview->results->percentiles.size())
            {
                plotAllPercentileData(diagramview->results->percentiles, "Percentiles");
                menu->addAction(QString("Plot percentiles"), this, SLOT(plotAllPercentileData()));
                menu->addSeparator();
            }
        }
        if (type == "Parameter" && diagramview->results)
        {
            if (diagramview->results)
            {
                if (diagramview->results->globalSensitivityMatrix.getnumrows())
                    menu->addAction(QString("Show global sensitivity matrix"), this, SLOT(showGlobalSensitivityMatrix()));
                if (diagramview->results->localSensitivityMatrix.getnumrows())
                    menu->addAction(QString("Show local sensitivity matrix"), this, SLOT(showLocalSensitivityMatrix()));
                if (diagramview->results->correlationMatrix.getnumrows())
                    menu->addAction(QString("Show correlation matrix"), this, SLOT(showCorrelationMatrix()));
                if (diagramview->results->localSensitivityMatrix.getnumrows() || diagramview->results->globalSensitivityMatrix.getnumrows() || diagramview->results->correlationMatrix.getnumrows())
                    menu->addSeparator();
            }
        }
#ifdef GIFMOD
/*		if (type == "Observation" && mainGraphWidget->results)
        {
            if (mainGraphWidget->modelSet != nullptr)
            {
                vector<CBTC> graphData;
                QMap<string, int> tracerIndex;
                QStringList tracerNames;
                for (int i = 0; i < mainGraphWidget->model->measured_quan.size(); i++)
                {
                    int tracerId = mainGraphWidget->model->measured_quan[i].quan;
                    string tracerName = mainGraphWidget->model->Tracer[tracerId].name;
                    tracerNames << QString::fromStdString(tracerName);
                    if (!tracerIndex.keys().contains(tracerName))
                    {
                        graphData.push_back(CBTC());
                        tracerIndex[tracerName] = graphData.size() - 1;
                    }
                    for (int j = 0; j < mainGraphWidget->model->measured_quan[i].observed_data.t.size(); j++)
                    {
                        double obs, mod;
                        obs = mainGraphWidget->model->measured_quan[i].observed_data.C[j];
                        mod = mainGraphWidget->model->ANS_obs.BTC[i].C[j];
                        graphData[tracerIndex[tracerName]].append(obs, mod);
                    }
                }
                tracerNames.removeDuplicates();
                vector<QString> names;
                names.resize(graphData.size());
                for (int i = 0; i < names.size(); i++)
                    names[tracerIndex[tracerNames[i].toStdString()]] = tracerNames[i];
                plotAgreementPlotDataforGroups(graphData, names);
                menu->addAction(QString("Plot Modeles vs Observations"), this, SLOT(plotAgreementPlotDataforGroups()));
                menu->addSeparator();
            }
        }
        */
#endif

#ifdef GWA
        if (type == "Observation" && mainGraphWidget->results)
        {
            if (mainGraphWidget->modelSet != nullptr)
            {
                vector<CBTC> graphData;
                QMap<string, int> tracerIndex;
                QStringList tracerNames;
                for (int i = 0; i < mainGraphWidget->model->measured_quan.size(); i++)
                {
                    int tracerId = mainGraphWidget->model->measured_quan[i].quan;
                    string tracerName = mainGraphWidget->model->Tracer[tracerId].name;
                    tracerNames << QString::fromStdString(tracerName);
                    if (!tracerIndex.keys().contains(tracerName))
                    {
                        graphData.push_back(CBTC());
                        tracerIndex[tracerName] = graphData.size() - 1;
                    }
                    for (int j = 0; j < mainGraphWidget->model->measured_quan[i].observed_data.t.size(); j++)
                    {
                        double obs, mod;
                        obs = mainGraphWidget->model->measured_quan[i].observed_data.C[j];
                        mod = mainGraphWidget->model->ANS_obs.BTC[i].C[j];
                        graphData[tracerIndex[tracerName]].append(obs, mod);
                    }
                }
                tracerNames.removeDuplicates();
                vector<QString> names;
                names.resize(graphData.size());
                for (int i = 0; i < names.size(); i++)
                    names[tracerIndex[tracerNames[i].toStdString()]] = tracerNames[i];
                plotAgreementPlotDataforGroups(graphData, names);
                menu->addAction(QString("Plot Modeles vs Observations"), this, SLOT(plotAgreementPlotDataforGroups()));
                menu->addSeparator();
            }
        }
#endif

        menu->addAction(QString("Add %1").arg(type) , this, SLOT(addProjectExplorerTreeItem()));
//

    }
    if (ui->treeView->indexAt(pos).data(Role::TreeItemType) == TreeItem::Type::NodeItem)
    {
        //QMenu *menu = new QMenu;
        Node *n = diagramview->node(ui->treeView->indexAt(pos).data().toString());

        TreeModel *model = projModel;
        TreeItem *item = model->itemFromIndex(ui->treeView->indexAt(pos));
        QString name = ui->treeView->indexAt(pos).data().toString();
        QModelIndex index = ui->treeView->indexAt(pos);
        removeProjectExplorerNodeItem(name, index); // model, type, index);
        menu->addAction(QString("Delete %1").arg(name), this, SLOT(removeProjectExplorerNodeItem()));
        QMap < QAction *, QStringList> menuKey;
        //diagramview->model = (diagramview->experimentID() == 0 || diagramview->modelSet->Medium.size() == 0) ? 0 : &(diagramview->modelSet->Medium[mainGraphWidget->experimentID() - 1]);
        QPoint pos2;
        pos2.setX(pos.x() + ui->dockWidget_props->x());
        pos2.setY(pos.y() + ui->dockWidget_props->y());
        diagramview->nodeContextMenuRequested(n,mapToGlobal(pos2), menu);

    }
    else if (ui->treeView->indexAt(pos).data(Role::TreeItemType) == TreeItem::Type::EdgeItem)
    {
        //QMenu *menu = new QMenu;
        Edge *n = diagramview->edge(ui->treeView->indexAt(pos).data().toString());
        TreeModel *model = projModel;
        TreeItem *item = model->itemFromIndex(ui->treeView->indexAt(pos));
        QString name = ui->treeView->indexAt(pos).data().toString();
        QModelIndex index = ui->treeView->indexAt(pos);
        removeProjectExplorerEdgeItem(name, index); // model, type, index);
        menu->addAction(QString("Delete %1").arg(name), this, SLOT(removeProjectExplorerEdgeItem()));
        QPoint pos2;
        pos2.setX(pos.x() + ui->dockWidget_props->x());
        pos2.setY(pos.y() + ui->dockWidget_props->y());
        diagramview->edgeContextMenuRequested(n, mapToGlobal(pos2), menu);
    }
    if (ui->treeView->indexAt(pos).data(Role::TreeItemType) == TreeItem::Type::Item)
    {
        //QMenu *menu = new QMenu;
        TreeModel *model = projModel;
        TreeItem *item = model->itemFromIndex(ui->treeView->indexAt(pos));
        if (item->Name() == "Markov chain Monte Carlo")
        {
            if (diagramview->results)
            {
                if (diagramview->results->globalSensitivityMatrix.getnumrows())
                    menu->addAction(QString("Show global sensitivity matrix"), this, SLOT(showGlobalSensitivityMatrix()));
                if (diagramview->results->localSensitivityMatrix.getnumrows())
                    menu->addAction(QString("Show local sensitivity matrix"), this, SLOT(showLocalSensitivityMatrix()));
                if (diagramview->results->correlationMatrix.getnumrows())
                    menu->addAction(QString("Show correlation matrix"), this, SLOT(showCorrelationMatrix()));
                if (diagramview->results->localSensitivityMatrix.getnumrows() || diagramview->results->globalSensitivityMatrix.getnumrows() || diagramview->results->correlationMatrix.getnumrows())
                    menu->exec(ui->treeView->mapToGlobal(pos));
            }
        }
    }
    if (ui->treeView->indexAt(pos).data(Role::TreeItemType) == TreeItem::Type::EntityItem)
    {
        //QMenu *menu = new QMenu;
        TreeModel *model = projModel;
        TreeItem *item = model->itemFromIndex(ui->treeView->indexAt(pos));
        QString name = ui->treeView->indexAt(pos).data().toString();


        if (item->parent()->Name() == "Observations")
        {
            bool addSeparator = false;
            Entity *obs = diagramview->entity(name, "Observation");
            QString file = obs->getValue("Observed data").toQString();
            CBTCSet data = CBTCSet(file.replace("./", modelPathname().append('/')).toStdString(), 1);
            if (data.nvars)
            {
                data[0].name = name.toStdString();
                plotObservationData(data[0], QString::fromStdString(data[0].name));
                menu->addAction(QString("Plot observation data").arg(name), this, SLOT(plotObservationData()));
                addSeparator = true;
            }
            else
            {   data = CTimeSeriesSet(1);
                data[0].name = "obs_1";
            }
            if (diagramview->modelSet != nullptr)
#ifdef GIFMOD
            if (mainGraphWidget->modelSet->ANS_obs.nvars)
            {
                int index = -1;
                for (int i = 0; i<mainGraphWidget->modelSet->ANS_obs.nvars;i++)
                    if (mainGraphWidget->modelSet->ANS_obs[i].name == name.toStdString())
                    {
                        plotModeledData(mainGraphWidget->modelSet->ANS_obs[i], data[0], QString("%1").arg(name));
                        menu->addAction(QString("Plot modeled data").arg(name), this, SLOT(plotModeledData()));

                        plotAgreementPlotData(data[0], mainGraphWidget->modelSet->ANS_obs[i].interpol(data[0]), QString("Agreement plot %1").arg(name));
                        menu->addAction(QString("Plot agreement plot").arg(name), this, SLOT(plotAgreementPlotData()));
                        addSeparator = true;
                    }
            }
#endif
#ifdef GWA
            if (mainGraphWidget->results->ANS_obs.nvars)
            {
                int index = -1;
                for (int i = 0; i<mainGraphWidget->results->ANS_obs.nvars; i++)
                    if (mainGraphWidget->results->ANS_obs[i].name == name.toStdString())
                    {
                        plotModeledDataDot(mainGraphWidget->results->ANS_obs[i], data[0], QString("%1").arg(name));
                        menu->addAction(QString("Plot Modeled Data").arg(name), this, SLOT(plotModeledDataDot()));

                        QString dataName = obs->val("tracer") + '@' + obs->val("well");
                        plotModeledData(mainGraphWidget->results->projected[dataName], data[0], QString("%1").arg(dataName));
                        menu->addAction(QString("Plot Projected Data").arg(name), this, SLOT(plotModeledData()));


                        plotAgreementPlotData(data[0], mainGraphWidget->results->ANS_obs[i].interpol(data[0]), QString("Agreement Plot %1").arg(name));
                        menu->addAction(QString("Plot Agreement Plot").arg(name), this, SLOT(plotAgreementPlotData()));
                        addSeparator = true;
                    }
            }
#endif

            if (addSeparator) menu->addSeparator();
            if (diagramview->results)
            {
//				mainGraphWidget->log(obs->name);
                if (diagramview->results->hasRealization(obs->name))
                {
                    plotRealization(diagramview->results->realization(obs->name), QString("Realization %1").arg(obs->name));
                    menu->addAction(QString("Plot realization data").arg(obs->name), this, SLOT(plotRealization()));
                    addSeparator = true;
                }
                if (diagramview->results->hasRealizationPercentile(obs->name))
                {
                    plotRealizationPercentile(diagramview->results->realizationPercentile(obs->name), QString("Realization percentile %1").arg(obs->name));
                    menu->addAction(QString("Plot prediction 95 percentile").arg(obs->name), this, SLOT(plotRealizationPercentile()));
                    addSeparator = true;
                }
                if (diagramview->results->hasNoiseRealization(obs->name))
                {
                    plotNoiseRealization(diagramview->results->noiseRealization(obs->name), QString("Realization (Noise) %1").arg(obs->name));
                    menu->addAction(QString("Plot realization (Noise)").arg(obs->name), this, SLOT(plotNoiseRealization()));
                    addSeparator = true;
                }
                if (diagramview->results->hasNoiseRealizationPercentile(obs->name))
                {
                    plotNoiseRealizationPercentile(diagramview->results->noiseRealizationPercentile(obs->name), QString("Realization percentile (Noise) %1").arg(obs->name));
                    menu->addAction(QString("Plot prediction 95 percentile (Noise)").arg(obs->name), this, SLOT(plotNoiseRealizationPercentile()));
                    addSeparator = true;
                }

            }
            if (addSeparator) menu->addSeparator();
        }

//		*********

        if (item->parent()->Name() == "Controllers")
        {
            bool addSeparator = false;
            Entity *controller = diagramview->entity(name, "Controller");
            //QString file = obs->getValue("Observed data").toQString();
            QString file = QString("./control_output_%1.txt").arg(diagramview->experimentName());
            CBTCSet data = CBTCSet(file.replace("./", modelPathname().append('/')).toStdString(), 1);
            if (data.nvars)
            {
                for (int i = 0; i < data.nvars; i++)
                {
                    if (data[i].name == controller->name.toStdString())
                    {
                        plotControllerData(data[i], QString::fromStdString(data[i].name));
                        menu->addAction(QString("Plot control data").arg(name), this, SLOT(plotControllerData()));
                        addSeparator = true;
                    }
                }
            }
            /*				if (mainGraphWidget->modelSet != nullptr)
                                if (mainGraphWidget->modelSet->ANS_obs.nvars)
                                {
                                    int index = -1;
                                    for (int i = 0; i<mainGraphWidget->modelSet->ANS_obs.nvars; i++)
                                        if (mainGraphWidget->modelSet->ANS_obs[i].name == name.toStdString())
                                        {
                                            plotModeledData(mainGraphWidget->modelSet->ANS_obs[i], data[0], QString("%1").arg(name));
                                            menu->addAction(QString("Plot modeled data").arg(name), this, SLOT(plotModeledData()));

                                            plotAgreementPlotData(data[0], mainGraphWidget->modelSet->ANS_obs[i].interpol(data[0]), QString("Agreement plot %1").arg(name));
                                            menu->addAction(QString("Plot agreement plot").arg(name), this, SLOT(plotAgreementPlotData()));
                                            addSeparator = true;
                                        }
                                }

                            if (addSeparator) menu->addSeparator();
                            if (mainGraphWidget->results)
                            {
                                //				mainGraphWidget->log(obs->name);
                                if (mainGraphWidget->results->hasRealization(obs->name))
                                {
                                    plotRealization(mainGraphWidget->results->realization(obs->name), QString("Realization %1").arg(obs->name));
                                    menu->addAction(QString("Plot realization data").arg(obs->name), this, SLOT(plotRealization()));
                                    addSeparator = true;
                                }
                                if (mainGraphWidget->results->hasRealizationPercentile(obs->name))
                                {
                                    plotRealizationPercentile(mainGraphWidget->results->realizationPercentile(obs->name), QString("Realization percentile %1").arg(obs->name));
                                    menu->addAction(QString("Plot prediction 95 percentile").arg(obs->name), this, SLOT(plotRealizationPercentile()));
                                    addSeparator = true;
                                }
                                if (mainGraphWidget->results->hasNoiseRealization(obs->name))
                                {
                                    plotNoiseRealization(mainGraphWidget->results->noiseRealization(obs->name), QString("Realization (Noise) %1").arg(obs->name));
                                    menu->addAction(QString("Plot realization (Noise)").arg(obs->name), this, SLOT(plotNoiseRealization()));
                                    addSeparator = true;
                                }
                                if (mainGraphWidget->results->hasNoiseRealizationPercentile(obs->name))
                                {
                                    plotNoiseRealizationPercentile(mainGraphWidget->results->noiseRealizationPercentile(obs->name), QString("Realization percentile (Noise) %1").arg(obs->name));
                                    menu->addAction(QString("Plot prediction 95 percentile (Noise)").arg(obs->name), this, SLOT(plotNoiseRealizationPercentile()));
                                    addSeparator = true;
                                }

                            }
                            if (addSeparator) menu->addSeparator();
                        }



                        ***************/
        }
        if (item->parent()->Name() == "Parameters" && diagramview->results)
        {
            bool addSeparator = false;
            Entity *par = diagramview->entity(name, "Parameter");
            if (diagramview->results->hasPercentile(par->name))
            {
                plotPercentileData(diagramview->results->percentile(par->name), par->name);
                menu->addAction(QString("Plot percentiles").arg(name), this, SLOT(plotPercentileData()));
                addSeparator = true;
            }
            if (diagramview->results->hasPrior(par->name))
            {
                plotPriorHistogram(diagramview->results->prior(par->name), QString ("Prior distribution of %1").arg(par->name));
                menu->addAction(QString("Plot prior distribution histogram").arg(name), this, SLOT(plotPriorHistogram()));
                addSeparator = true;
            }
            if (diagramview->results->hasPosterior(par->name))
            {
                plotPosteriorHistogram(diagramview->results->posterior(par->name), QString("Posterior distribution of %1").arg(par->name));
                menu->addAction(QString("Plot posterior distribution histogram").arg(name), this, SLOT(plotPosteriorHistogram()));
                addSeparator = true;
            }
            if (addSeparator) menu->addSeparator();
        }
        QModelIndex index = ui->treeView->indexAt(pos);
        removeProjectExplorerEntityItem(name, index); // model, type, index);
        menu->addAction(QString("Delete %1").arg(name), this, SLOT(removeProjectExplorerEntityItem()));
        menu->exec(ui->treeView->mapToGlobal(pos));
    }
    if (ui->treeView->indexAt(pos).data(Role::TreeItemType) == TreeItem::Type::ReactionNetworkItem)
    {
        //QMenu *menu = new QMenu;
        menu->addAction(QString("Open reaction network window"), this, SLOT(openRXNWindow()));
        menu->exec(ui->treeView->mapToGlobal(pos));
    }
    if (ui->treeView->indexAt(pos).data(Role::TreeItemType) != TreeItem::Type::EdgeItem && ui->treeView->indexAt(pos).data(Role::TreeItemType) != TreeItem::Type::NodeItem)
        menu->exec(ui->treeView->mapToGlobal(pos));
}

void MainWindow::addProjectExplorerTreeItem(TreeModel *model, const QString type,const QModelIndex index)
{
    static TreeModel *_model;
    static QString _type;
    static QModelIndex _index;
    if (model != nullptr)
    {
        _model = model;
        _type = type;
        _index = index;
        return;
    }
    else
    {
        _model->addChildFromMenu(_type, &_index);
        ui->treeView->setExpanded(_index, true);
    }
}

void MainWindow::plotAllPercentileData(vector<percentileData> data, QString name)
{
    static vector<percentileData> _data;
    static QString _name;
    if (data.size())
    {
        _data = data;
        _name = name;
        return;
    }
    else
    {
        plotWindow *plot = new plotWindow(diagramview);
        for (unsigned int i = 0; i < _data.size(); i++)
        {
            QMap<QString, double> mapData;
            mapData["p25"] = _data[i].p25;
            mapData["p50"] = _data[i].p50;
            mapData["p975"] = _data[i].p975;
            mapData["mean"] = _data[i].mean;

            plot->addPercentilePlot(QString::fromStdString(_data[i].parameter), mapData);
        }
        plot->show();
    }
}

void MainWindow::removeProjectExplorerNodeItem(QString name, const QModelIndex index)
{
    static QString _name;
    static QModelIndex _index;

    if (!name.isEmpty())
    {
        _name = name;
        _index = index;
        return;
    }
    else
    {
        QModelIndex i = ui->treeView->indexAbove(_index.sibling(0, 0));
        ui->treeView->collapse(i);
        QModelIndex j = ui->treeView->indexBelow(i);
        bool connectors_expanded = ui->treeView->isExpanded(j);
        ui->treeView->collapse(j);
        projModel->deleteNode(_index.row(), i, _name);
        if (projModel->itemFromIndex(i)->childCount())
            ui->treeView->expand(i);

        if (connectors_expanded && projModel->connectors->childCount())
            ui->treeView->expand(j);
    }
}
void MainWindow::removeProjectExplorerEdgeItem(QString name, const QModelIndex index)
{
    static QString _name;
    static QModelIndex _index;

    if (!name.isEmpty())
    {
        _name = name;
        _index = index;
        return;
    }
    else
    {
        QModelIndex i = ui->treeView->indexAbove(_index.sibling(0, 0));
        ui->treeView->collapse(i);
        projModel->deleteEdge(_index.row(), i, _name);
        if (projModel->itemFromIndex(i)->childCount())
            ui->treeView->expand(i);
    }
}
void MainWindow::removeProjectExplorerEntityItem(QString name, const QModelIndex index)
{
    static QString _name;
    static QModelIndex _index;

    if (!name.isEmpty())
    {
        _name = name;
        _index = index;
        return;
    }
    else
    {
        QModelIndex i = ui->treeView->indexAbove(_index.sibling(0, 0));
        ui->treeView->collapse(i);
        QString type = diagramview->entity(_name)->objectType.ObjectType;
        projModel->deleteEntity(_index.row(), i, type, _name);
        if (projModel->itemFromIndex(i)->childCount())
            ui->treeView->expand(i);
    }
}


/*void MainWindow::on_tableProp_customContextMenuRequested(const QPoint &pos)
{
    int i = 0;
    /*
    if (projectExplorer->indexAt(pos).data(Role::TreeItemType) == TreeItem::Type::Branch)
    {
        QMenu *menu = new QMenu;
        TreeModel *model = projModel;
        TreeItem *item = model->itemFromIndex(projectExplorer->indexAt(pos));
        QString type = projectExplorer->indexAt(pos).data().toString();
        QModelIndex index = projectExplorer->indexAt(pos);
        addProjectExplorerTreeItem(model, type, index);
        menu->addAction(QString("Add %1").arg(type), this, SLOT(addProjectExplorerTreeItem()));
        menu->exec(projectExplorer->mapToGlobal(pos));
    }
    if (projectExplorer->indexAt(pos).data(Role::TreeItemType) == TreeItem::Type::ReactionNetworkItem)
    {
        QMenu *menu = new QMenu;
        menu->addAction(QString("Open Reaction Network Window"), this, SLOT(openRXNWindow()));
        menu->exec(projectExplorer->mapToGlobal(pos));
    }

}*/

void MainWindow::plotPriorHistogram(CBTC histogram, QString name)
{
    static CBTC data;
    static QString _name;
    if (histogram.n)
    {
        data = histogram;
        _name = name;
        return;
    }
    else
    {
        plotWindow *plot = new plotWindow(diagramview);
        plot->addHistogramPlot(_name, data.t, data.C);
        plot->show();
    }
}
void MainWindow::plotPosteriorHistogram(CBTC histogram, QString name)
{
    static CBTC data;
    static QString _name;
    if (histogram.n)
    {
        data = histogram;
        _name = name;
        return;
    }
    else
    {
        plotWindow *plot = new plotWindow(diagramview);
        plot->addHistogramPlot(_name, data.t, data.C);
        plot->show();
    }
}

void MainWindow::plotPercentileData(percentileData data, QString name)
{
    static percentileData _data;
    static QString _name;
    if (data.parameter !="")
    {
        _data = data;
        _name = name;
        return;
    }
    else
    {
        QMap<QString, double> mapData;
        mapData["p25"] = _data.p25;
        mapData["p50"] = _data.p50;
        mapData["p975"] = _data.p975;
        mapData["mean"] = _data.mean;

        plotWindow *plot = new plotWindow(diagramview);
        plot->addPercentilePlot(_name, mapData);
        plot->show();
    }
}

void MainWindow::plotObservationData(CBTC data, QString name)
{
    static CBTC _data;
    static QString _name;
    if (data.n)
    {
        _data = data;
        _name = name;
        return;
    }
    else
    {
        plotWindow *plot = new plotWindow(diagramview);
        bool convertTime = true;
#ifdef GWA
        convertTime = false;
#endif
        plotformat format;
        format.xAxisTimeFormat = convertTime;
        plot->addScatterPlot(_data, _name + "(Observation)", format);
        //plot->addScatterPlot(_data, _name + "(Observation)", convertTime);
        plot->show();
    }
    return;
}

void MainWindow::plotControllerData(CBTC data, QString name)
{
    static CBTC _data;
    static QString _name;
    if (data.n)
    {
        _data = data;
        _name = name;
        return;
    }
    else
    {
        plotWindow *plot = new plotWindow(diagramview);
        bool convertTime = true;
        plot->addScatterPlot(_data, _name + "(Controll)", convertTime);
        plot->show();
    }
}

void MainWindow::plotModeledData(CBTC modeled, CBTC observed, QString _name)
{
    static CBTC model, obs;
    static QString name;
    if (modeled.n)
    {
        model = modeled;
        obs = observed;
        name = _name;
        return;
    }
    else
    {
        plotWindow *plot = new plotWindow(diagramview);
        plotformat format, format2;
        format.penStyle = Qt::SolidLine;
        format.lineStyle = QCPGraph::LineStyle::lsNone;
        format.color = Qt::blue;
        format.scatterStyle = QCPScatterStyle::ssPlusCircle;
        format.xAxisTimeFormat = true;
#ifdef GWA
        format.xAxisTimeFormat = false;
#endif
        if (obs.n>1)
            plot->addScatterPlot(obs, name + "(Observed)", format);

        format2.penStyle = Qt::SolidLine;
        format2.scatterStyle = QCPScatterStyle::ssNone;
        format2.xAxisTimeFormat = true;
        plot->addScatterPlot(model, name, format2);
        plot->show();
    }
}
void MainWindow::plotModeledDataDot(CBTC modeled, CBTC observed, QString _name)
{
    static CBTC model, obs;
    static QString name;
    if (modeled.n)
    {
        model = modeled;
        obs = observed;
        name = _name;
        return;
    }
    else
    {
        plotWindow *plot = new plotWindow(diagramview);
        plotformat format, format2;
        format.penStyle = Qt::SolidLine;
        format.lineStyle = QCPGraph::LineStyle::lsNone;
        format.color = Qt::blue;
        format.scatterStyle = QCPScatterStyle::ssPlusCircle;
        format.xAxisTimeFormat = true;
#ifdef GWA
        format.xAxisTimeFormat = false;
#endif
        plot->addScatterPlot(obs, name + "(Observation)", format);

        format2.penStyle = Qt::SolidLine;
        format2.scatterStyle = QCPScatterStyle::ssPlusCircle;
        format2.xAxisTimeFormat = true;
        plot->addScatterPlot(model, name + "(Modeled)", format2);
        plot->show();
    }
}
void MainWindow::plotAgreementPlotData(CBTC observation, CBTC modeled, QString name)
{
    static CBTC _obs;
    static CBTC _mod;
    static QString _name;
    if (modeled.n)
    {
        _obs = observation;
        _mod = modeled;
        _name = name;
        return;
    }
    else
    {
        plotWindow *plot = new plotWindow(diagramview);
        map<string, double> reg = regression(_obs.C, _mod.C);
        if (reg["error"] == 0)
        {
            plotformat format, format2;
            format.lineStyle = QCPGraph::LineStyle::lsNone;
            format.scatterStyle = QCPScatterStyle::ssCross;
            format.color = Qt::blue;
            format.xAxisLabel = "Observation";
            format.yAxisLabel = "Modeled";
            format.xAxisTimeFormat = true;

            plot->addDotPlot(_obs.C, _mod.C, "Agreement Plot", format);
            CBTC regLine(reg["a"], reg["b"], _obs.C);
            format2.scatterStyle = QCPScatterStyle::ssNone;
            format2.xAxisTimeFormat = false;
            plot->addScatterPlot(regLine, "Agreement regression", format2);
            plot->show();
        }
    }
}
void MainWindow::plotAgreementPlotDataforGroups(vector<CBTC> obs_modData, vector<QString> names)
{
    static vector<CBTC> _obs_modData;
    static vector<QString> _names;
    if (obs_modData.size())
    {
        _obs_modData = obs_modData;
        _names = names;
        return;
    }
    else
    {
        QList<QCPScatterStyle::ScatterShape> shapes;
        shapes << QCPScatterStyle::ssCross << QCPScatterStyle::ssPlus << QCPScatterStyle::ssCircle << QCPScatterStyle::ssDisc << QCPScatterStyle::ssSquare
            << QCPScatterStyle::ssDiamond << QCPScatterStyle::ssStar << QCPScatterStyle::ssTriangle << QCPScatterStyle::ssTriangleInverted
            << QCPScatterStyle::ssCrossSquare << QCPScatterStyle::ssPlusSquare << QCPScatterStyle::ssCrossCircle << QCPScatterStyle::ssPlusCircle
            << QCPScatterStyle::ssPeace;
        QList<Qt::GlobalColor> colors;
        colors << Qt::blue << Qt::red << Qt::magenta << Qt::cyan << Qt::darkYellow << Qt::gray << Qt::darkCyan << Qt::darkRed << Qt::darkBlue;

        int shapeCounter = -1, colorCounter = 0;
        plotWindow *plot = new plotWindow(diagramview);
        //map<string, double> reg = regression(_obs.C, _mod.C);
        //if (reg["error"] == 0)
        //{
            plotformat format, format2;
            format.lineStyle = QCPGraph::LineStyle::lsNone;

            format.xAxisLabel = "Observation";
            format.yAxisLabel = "Modeled";
            format.xAxisTimeFormat = true;
            format.yAxisType = QCPAxis::stLogarithmic;
            format.penStyle = Qt::SolidLine;
            for (unsigned int i = 0; i < _names.size(); i++)
            {
                if (++shapeCounter == shapes.size())
                {
                    shapeCounter = 0;
                    colorCounter++;
                }
                format.scatterStyle = shapes[shapeCounter];
                format.color = colors[colorCounter];
                plot->addDotPlot(_obs_modData[i].t, _obs_modData[i].C, _names[i], format);
            }
//			CBTC regLine(reg["a"], reg["b"], _obs.C);
//			format2.scatterStyle = QCPScatterStyle::ssNone;
//			format2.xAxisTimeFormat = false;
//			plot->addScatterPlot(regLine, "Agreement regression", format2);
            plot->show();
        //}
    }
}
void MainWindow::plotRealization(CBTCSet data, QString name)
{
    //qDebug() << "called";
    static CBTCSet _data;
    static QString _name;
    //qDebug() << data.nvars;
    if (data.nvars)
    {
        _data = data;
        _name = name;
        return;
    }
    else
    {
        plotWindow *plot = new plotWindow(diagramview);
        plotformat format;
        format.legend = false;
        format.penWidth = 1;
        format.xAxisTimeFormat = true;
#ifdef GWA
        format.xAxisTimeFormat = false;
#endif
        for (int i = 0; i < _data.nvars; i++)
        {
            //qDebug() << QString::fromStdString(_data[i].name);
            plot->addScatterPlot(_data[i], QString::fromStdString(_data[i].name), format);
        }
        plot->show();
    }
}

void MainWindow::plotNoiseRealization(CBTCSet data, QString name)
{
    static CBTCSet _data;
    static QString _name;
    if (data.nvars)
    {
        _data = data;
        _name = name;
        return;
    }
    else
    {
        plotWindow *plot = new plotWindow(diagramview);
        bool convertTime = true;
#ifdef GWA
        convertTime = false;
#endif
        for (int i = 0; i < _data.nvars; i++)
        {
            plot->addScatterPlot(_data[i], QString::fromStdString(_data[i].name), convertTime);
        }
        plot->show();
    }
}
void MainWindow::plotRealizationPercentile(CBTCSet data, QString name)
{
    static CBTCSet _data;
    static QString _name;
    if (data.nvars)
    {
        _data = data;
        _name = name;
        return;
    }
    else
    {
        plotWindow *plot = new plotWindow(diagramview);
        QCPGraph *g1 = nullptr;
        QCPGraph *g2 = nullptr;
        bool convertTime = true;
#ifdef GWA
        convertTime = false;
#endif

        for (int i = 0; i < _data.nvars; i++)
        {
            plotformat format;
            format.penWidth = 1;
            format.color = Qt::GlobalColor(i);
            format.xAxisTimeFormat = convertTime;
            if (i == _data.nvars - 1)
                format.fillGraph = g1;
            g2 = plot->addScatterPlot(_data[i], QString::fromStdString(_data[i].name), format);
            if (i == 1)
                g1 = g2;
        }
        plot->show();
    }
}
void MainWindow::plotNoiseRealizationPercentile(CBTCSet data, QString name)
{
    static CBTCSet _data;
    static QString _name;
    if (data.nvars)
    {
        _data = data;
        _name = name;
        return;
    }
    else
    {
        plotWindow *plot = new plotWindow(diagramview);
//		for (int i = 0; i < _data.nvars; i++)
//		{
//			plot->addScatterPlot(_data[i], QString::fromStdString(_data[i].name), true);
//		}
        bool convertTime = true;
#ifdef GWA
        convertTime = false;
#endif

        plot->addScatterPlot(_data, convertTime);
        plot->show();
    }
}
