#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QList>
#include "System.h"
#include "diagramviewer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QList<QAction*> actions;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    System system;
    QMap<QString,int> counts;
    View *diagramview;
    QGraphicsScene *scene;
private slots:
    void onaddlink();
    void onaddblock();

};

#endif // MAINWINDOW_H
