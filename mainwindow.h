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
    QStringList recentFiles;
    bool saveModel(QString &fileName);
    void setModelFileName(const QString &fileName);
    void addToRecentFiles(QString fileName, bool addToFile=true);
    void writeRecentFilesList();
private slots:
    void onaddlink();
    void onaddblock();
    void on_action_Save_triggered();

};

#endif // MAINWINDOW_H
