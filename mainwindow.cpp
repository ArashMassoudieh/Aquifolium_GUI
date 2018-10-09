#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "QGraphicsRectItem"
#include "aqflmblockitem.h"
#include "logwindow.h"
#include "node.h"
#include "edge.h"
#include "delegate.h"

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
