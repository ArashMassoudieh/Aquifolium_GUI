#pragma once
#include <QMainWindow>
#include <QAction>
#include <QList>
#include "GWidget.h"
#include "treemodel.h"
#include "runtimeWindow.h"
#include "System.h"

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
    QString applicationName;
    QString &modelPathname() {return modelpathname;}
    System *GetSystem() {return &system;}
    runtimeWindow *rtw;
private:
    Ui::MainWindow *ui;
    System system;
    QMap<QString,int> counts;
    GraphWidget *diagramview;
    QGraphicsScene *scene;
    QString modelpathname;
    string modelfilename;
    string entitiesfilename;//the name of the JSON containing general properities of the system
    TreeModel *projModel;
    QStringList recentFiles;
    bool saveModel(QString &fileName);
    bool saveModelJson(QString &fileName);
    void setModelFileName(const QString &fileName);
    void addToRecentFiles(QString fileName, bool addToFile=true);
    void writeRecentFilesList();
    bool loadModel(QString modelfilename);
    bool ReadEntitiesJson();
    QString fileExtension, metafilename, applicationShortName;
    void forwardRun(System *model, runtimeWindow* progress);
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
    void on_actionSave_As_JSON_triggered();
    void on_actionRun_Model_triggered();
};


