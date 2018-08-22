#ifndef DIAGRAMVIEWER_H
#define DIAGRAMVIEWER_H

#include <QFrame>
#include <QGraphicsView>

class QLabel;
class QSlider;
class QToolButton;

class View;

class DiagramViewer: public QGraphicsView
{
public:
    Q_OBJECT
   public:
       DiagramViewer(View *v) : QGraphicsView(), view(v) { }

   protected:
   #if QT_CONFIG(wheelevent)
       void wheelEvent(QWheelEvent *) override;
   #endif

   private:
       View *view;
   };

   class View : public QFrame
   {
       Q_OBJECT
   public:
       explicit View(const QString &name, QWidget *parent = 0);

       QGraphicsView *view() const;

   public slots:
       void zoomIn(int level = 1);
       void zoomOut(int level = 1);

   private slots:
       void resetView();
       void setResetButtonEnabled();
       void setupMatrix();
       void togglePointerMode();
       void toggleOpenGL();
       void toggleAntialiasing();
       void print();
       void rotateLeft();
       void rotateRight();

   private:
       DiagramViewer *diagramView;
       QLabel *label;
       QLabel *label2;
       QToolButton *selectModeButton;
       QToolButton *dragModeButton;
       QToolButton *openGlButton;
       QToolButton *antialiasButton;
       QToolButton *printButton;
       QToolButton *resetButton;
       QSlider *zoomSlider;
       QSlider *rotateSlider;
};

#endif // DIAGRAMVIEWER_H
