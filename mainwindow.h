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
    bool loadModel(QString modelfilename);
    QString fileExtension, metafilename, applicationShortName;
private slots:
    void onaddlink();
    void onaddblock();
    void on_action_Save_triggered();
    void on_actionZoom_In_triggered();
    void on_actionZoom_Out_triggered();
    void on_actionZoom_All_triggered();
    void on_actionE_xit_triggered();
    void on_action_Open_triggered();
    void on_actionSave_As_triggered();
};

#endif // MAINWINDOW_H
