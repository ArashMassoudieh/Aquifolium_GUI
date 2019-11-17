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
	bool saveModel_to_script(QString& fileName);
    bool saveModelJson(QString &fileName);
    void setModelFileName(const QString &fileName);
    void addToRecentFiles(QString fileName, bool addToFile=true);
    void writeRecentFilesList();
    bool loadModel(QString modelfilename);
    bool loadModelJSON(QString modelfilename);
    bool ReadEntitiesJson();
    QString fileExtension, metafilename, applicationShortName;
    void forwardRun(System *model, runtimeWindow* progress);
    void showHelp(int code, QString variableName);

    QModelIndex addParameterIndex(const QModelIndex &index = QModelIndex());
    void addParameter(QAction* item = nullptr);
    void addProjectExplorerTreeItem(TreeModel *model = nullptr, const QString name = "", const QModelIndex = QModelIndex());
	bool BuildObjectsToolBar(); 

    //plots:
    void plotObservationData(CBTC data = CBTC(), QString name = "");
    void plotControllerData(CBTC data = CBTC(), QString name = "");
    void plotModeledData(CBTC modeled = CBTC(), CBTC observed = CBTC(), QString name = "");
    void plotModeledDataDot(CBTC modeled = CBTC(), CBTC observed = CBTC(), QString name = "");
    void plotAgreementPlotData(CBTC observation = CBTC(), CBTC modeled = CBTC(), QString name = "");
    void plotAgreementPlotDataforGroups(vector<CBTC> obs_modData = vector<CBTC>(), vector<QString> names = vector<QString>());
    void plotPercentileData(percentileData data = percentileData(), QString name = "");
    void plotAllPercentileData(vector<percentileData> data = vector<percentileData>(), QString name = "");
    void plotPriorHistogram(CBTC histogram = CBTC(), QString name = "");
    void plotPosteriorHistogram(CBTC histogram = CBTC(), QString name = "");
    void plotRealization(CBTCSet data = CBTCSet(), QString name = "");
    void plotNoiseRealization(CBTCSet data = CBTCSet(), QString name = "");
    void plotRealizationPercentile(CBTCSet data = CBTCSet(), QString name = "");
    void plotNoiseRealizationPercentile(CBTCSet data = CBTCSet(), QString name = "");
    void plotTimeSeries(QAction*, CBTC data = CBTC(), QString name = "", bool convertXtoTime = true, bool reset = false);
private slots:
    void onaddlink();
    void onaddblock();
	void onaddsource(); 
	void onaddentity();
    void on_action_Save_triggered();
    void on_actionZoom_In_triggered();
    void on_actionZoom_Out_triggered();
    void on_actionZoom_All_triggered();
    void on_actionE_xit_triggered();
    void on_action_Open_triggered();
    void on_actionSave_As_triggered();
	void on_actionSave_As_Script_triggered();
    void on_actionSave_As_JSON_triggered();
    void on_actionRun_Model_triggered();
    void on_projectExplorer_clicked(const QModelIndex& index);
    void tablePropShowContextMenu(const QPoint&pos);
    void getNumber(double initial=-1);
    void on_projectExplorer_customContextMenuRequested(const QPoint &pos);
    void removeProjectExplorerNodeItem(QString name = "", const QModelIndex = QModelIndex());
    void removeProjectExplorerEdgeItem(QString name = "", const QModelIndex = QModelIndex());
    void removeProjectExplorerEntityItem(QString name = "", const QModelIndex = QModelIndex());

};


