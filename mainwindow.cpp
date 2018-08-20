#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#define cout qDebug()

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    system.GetQuanTemplate(qApp->applicationDirPath().toStdString() + "/resources/power_reservoirs.qnt");
    ui->setupUi(this);
    qDebug()<<qApp->applicationDirPath();
    qDebug()<<"Salam!";
    qDebug()<<"Salam!";

    for (int i=0; i<system.Get)->size(); i++)
    {
        QAction* action = new QAction(this);
        action->setObjectName();
        QIcon icon;
        icon.addFile(QString::fromStdString(qApp->applicationDirPath().toStdString()+"/resources/Icons/farm.png"), QSize(), QIcon::Normal, QIcon::Off);
        action->setIcon(icon);
        ui->mainToolBar->addAction(action);
        action->setText(QApplication::translate("MainWindow", "Text!!!!", nullptr));
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
