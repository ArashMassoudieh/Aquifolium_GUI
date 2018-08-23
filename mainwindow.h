#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QList>
#include "System.h"
#include "diagramviewer.h"

class AqflmBlockItem;

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
    void additemselected(AqflmBlockItem *bitm) {itemselected = bitm;}
private:
    Ui::MainWindow *ui;
    System system;
    QMap<QString,int> counts;
    View *diagramview;
    QGraphicsScene *scene;
    AqflmBlockItem *itemselected;
private slots:
    void onaddlink();
    void onaddblock();

};

#endif // MAINWINDOW_H
