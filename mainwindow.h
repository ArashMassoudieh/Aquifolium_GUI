#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QList>
#include "System.h"
#include "diagramviewer.h"
#include "GWidget.h"
#include "treemodel.h"

class AqflmBlockItem;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QList<QAction*> actions;
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void additemselected(AqflmBlockItem *bitm) {itemselected = bitm;}
    QString applicationName;
    QString &modelPathname() {return modelpathname;}
    System *GetSystem() {return &system;}
private:
    Ui::MainWindow *ui;
    System system;
    QMap<QString,int> counts;
    GraphWidget *diagramview;
    QGraphicsScene *scene;
    AqflmBlockItem *itemselected;
    QString modelpathname;
    TreeModel *projModel;
private slots:
    void onaddlink();
    void onaddblock();

};

#endif // MAINWINDOW_H
