#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include "PKGModel.h"

namespace Ui
{
class MainWindow;
}

namespace Rangers
{
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    QGraphicsScene scene;
    QGraphicsPixmapItem item;

    QList<QPixmap> frames;
    QTimer animationTimer;
    int currentFrame;

    void loadFile(const QString& fileName);
    PKGModel *model;

private Q_SLOTS:
    void openFile();
    void nextFrame();
    void framerateChanged(double value);
    void durationChanged(double value);
};
}
#endif // MAINWINDOW_H
