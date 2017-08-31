#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "iostream"
#include "Logic.h"
#include "qcustomplot.h"
#include <unistd.h>
#include "cstdio"
#include <QTimer>

#define THICKNESS 10

#define MassDefault 1.80
#define KDefault 40
#define X1Default -20
#define X2Default -5
#define BlockWidth 100
#define BlockHeight 100
#define Scale 3

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_massSlider_sliderMoved(int position);

    void on_kSlider_sliderMoved(int position);

    void on_x1Slider_sliderMoved(int position);

    void on_x2Slider_sliderMoved(int position);

    void slotAlarmTimer();

private:
    Ui::MainWindow *ui;
    /* Объявляем графическую сцену и Точку с которой будем работать */
    QGraphicsScene  *scene;
    Logic* logic;
    double time;
    bool started;
    QVector<double>* steps;
    QVector<double>* values1;
    QVector<double>* values2;
    QCustomPlot *customPlot;    // Объявляем графическое полотно
    QCPGraph *graphic1;          // Объявляем график
    QCPGraph *graphic2;          // Объявляем график
    QTimer *timer;

    int MAXx;
    int MAXy;

    int blockX1;
    int blockX2;
    int blockY;
    int springY;
    int springHeight;

    void draw();

    double x1;
    double x2;
};

#endif // MAINWINDOW_H
