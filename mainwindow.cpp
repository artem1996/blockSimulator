#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MAXx = (ui->graphicsView->width() - 4) / 2;
    MAXy = (ui->graphicsView->height() - 4) / 2;
   time = 0;
   logic = new Logic(MassDefault, KDefault, X1Default, X2Default);
   customPlot = new QCustomPlot(); // Инициализируем графическое полотно
   ui->gridLayout->addWidget(customPlot,0,0,1,1);  // Устанавливаем customPlot в окно проложения
   ui->kSlider->setMinimum(1);
   ui->kSlider->setMaximum(100);
   ui->massSlider->setMinimum(20);
   ui->massSlider->setMaximum(500);
   ui->x1Slider->setMinimum(-25);
   ui->x1Slider->setMaximum(25);
   ui->x2Slider->setMinimum(-25);
   ui->x2Slider->setMaximum(25);
   char str[8];
   sprintf(str, "%.2f", MassDefault);
   ui->massLabel->setText(str);
   ui->massSlider->setValue(MassDefault * 100);
   sprintf(str, "%d", KDefault);
   ui->kLabel->setText(str);
   ui->kSlider->setValue(KDefault);
   sprintf(str, "%d", X1Default);
   ui->x1Label->setText(str);
   ui->x1Slider->setValue(X1Default);
   sprintf(str, "%d", X2Default);
   ui->x2Label->setText(str);
   ui->x2Slider->setValue(X2Default);
   customPlot->setInteraction(QCP::iRangeZoom,true);   // Включаем взаимодействие удаления/приближения
   customPlot->setInteraction(QCP::iRangeDrag, true);  // Включаем взаимодействие перетаскивания графика
   customPlot->axisRect()->setRangeDrag(Qt::Horizontal);   // Включаем перетаскивание только по горизонтальной оси
   customPlot->axisRect()->setRangeZoom(Qt::Horizontal);   // Включаем удаление/приближение только по горизонтальной оси
   customPlot->yAxis->setRange(-50, 50);
   customPlot->xAxis->setRange(0, 30);
   values1 = new QVector<double>();
   steps = new QVector<double>();
   values2 = new QVector<double>();
   graphic1 = new QCPGraph(customPlot->xAxis, customPlot->yAxis);
   customPlot->addPlottable(graphic1);  // Устанавливаем график на полотно
   graphic1->setName("Левый");       // Устанавливаем
   graphic1->setPen(QPen(Qt::green)); // Устанавливаем цвет графика
   graphic2 = new QCPGraph(customPlot->xAxis, customPlot->yAxis);
   customPlot->addPlottable(graphic2);  // Устанавливаем график на полотно
   graphic2->setName("Правый");       // Устанавливаем
   graphic2->setPen(QPen(Qt::blue)); // Устанавливаем цвет графика
   timer = new QTimer();               // Инициализируем Таймер
       // Подключаем СЛОТ для отрисовки к таймеру
       connect(timer, SIGNAL(timeout()), this, SLOT(slotAlarmTimer()));

   started = false;

   blockX1 = MAXx * 2 / 3 - MAXx - BlockWidth / 2;
   blockX2 = MAXx * 4 / 3 - MAXx - BlockWidth / 2;
   blockY = MAXy - BlockHeight - THICKNESS;
   springHeight = BlockHeight / 3;
   springY = MAXy - THICKNESS - springHeight;
   x1 = X1Default;
   x2 = X2Default;
   draw();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if(started) {
        timer->stop();
        ui->pushButton->setText("Старт");
    } else {
        customPlot->removeGraph(graphic1);
        customPlot->removeGraph(graphic2);
        delete values1;
        delete values2;
        delete steps;
        values1 = new QVector<double>();
        steps = new QVector<double>();
        values2 = new QVector<double>();
        graphic1 = new QCPGraph(customPlot->xAxis, customPlot->yAxis);
        customPlot->addPlottable(graphic1);  // Устанавливаем график на полотно
        graphic1->setName("Левый");       // Устанавливаем
        graphic1->setPen(QPen(Qt::green)); // Устанавливаем цвет графика
        graphic2 = new QCPGraph(customPlot->xAxis, customPlot->yAxis);
        customPlot->addPlottable(graphic2);  // Устанавливаем график на полотно
        graphic2->setName("Правый");       // Устанавливаем
        graphic2->setPen(QPen(Qt::blue)); // Устанавливаем цвет графика
        time = 0;
        timer->start(50);
        ui->pushButton->setText("Стоп");
    }
    started = !started;
}

void MainWindow::on_massSlider_sliderMoved(int position)
{
    char str[8];
    sprintf(str, "%.2f", (double) position / 100);
    ui->massLabel->setText(str);
    logic->setMu((double) position / 100);
}

void MainWindow::on_kSlider_sliderMoved(int position)
{
    char str[8];
    sprintf(str, "%d", position);
    ui->kLabel->setText(str);
    logic->setK(position);
}

void MainWindow::on_x1Slider_sliderMoved(int position)
{
    char str[8];
    sprintf(str, "%d", position);
    ui->x1Label->setText(str);
    logic->setX10(position);
    if(!started) {
        x1 = position;
        draw();
    }
}

void MainWindow::on_x2Slider_sliderMoved(int position)
{
    char str[8];
    sprintf(str, "%d", position);
    ui->x2Label->setText(str);
    logic->setX20(position);
    if(!started) {
        x2 = position;
        draw();
    }
}

void MainWindow::slotAlarmTimer()
{
    for(int i = 0; i < 5; i++) {
        x1 = logic->getX1(time);
        x2 = logic->getX2(time);

        steps->push_back(time);
        values1->push_back(x1);
        values2->push_back(x2);

    time += 0.01;
    }
    draw();
}

void MainWindow::draw()
{
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    QGraphicsRectItem* item1 = new QGraphicsRectItem(blockX1 + x1 * Scale, blockY, BlockWidth, BlockHeight);
    item1->setBrush(QBrush(Qt::green));
    scene->addItem(item1);
    QGraphicsRectItem* item2 = new QGraphicsRectItem(blockX2 + x2 * Scale, blockY, BlockWidth, BlockHeight);
    item2->setBrush(QBrush(Qt::blue));
    scene->addItem(item2);
    QGraphicsRectItem* item3 = new QGraphicsRectItem(-MAXx, -MAXy, THICKNESS, MAXy * 2);
    item3->setBrush(QBrush(Qt::red));
    scene->addItem(item3);
    QGraphicsRectItem* item4 = new QGraphicsRectItem(MAXx - THICKNESS, -MAXy, THICKNESS, MAXy * 2);
    item4->setBrush(QBrush(Qt::red));
    scene->addItem(item4);
    QGraphicsRectItem* item5 = new QGraphicsRectItem(- MAXx, MAXy - THICKNESS, MAXx * 2, THICKNESS);
    item5->setBrush(QBrush(Qt::red));
    scene->addItem(item5);
    double springStart = -MAXx + THICKNESS;
    double springStep = (blockX1 + x1 * Scale - springStart) / 10.0;
    for(int i = 0; i < 5; i++) {
        scene->addLine(springStart, springY, springStart + springStep, springY - springHeight);
        springStart += springStep;
        scene->addLine(springStart, springY - springHeight, springStart + springStep, springY);
        springStart += springStep;
    }
    springStart = blockX1 + x1 * Scale + BlockWidth;
    springStep = (blockX2 + x2 * Scale - springStart) / 10.0;
    for(int i = 0; i < 5; i++) {
        scene->addLine(springStart, springY, springStart + springStep, springY - springHeight);
        springStart += springStep;
        scene->addLine(springStart, springY - springHeight, springStart + springStep, springY);
        springStart += springStep;
    }
    springStart = blockX2 + x2 * Scale + BlockWidth;
    springStep = (MAXx - THICKNESS - springStart) / 10.0;
    for(int i = 0; i < 5; i++) {
        scene->addLine(springStart, springY, springStart + springStep, springY - springHeight);
        springStart += springStep;
        scene->addLine(springStart, springY - springHeight, springStart + springStep, springY);
        springStart += springStep;
    }
    graphic1->setData(*steps, *values1); // Устанавливаем данные
    graphic2->setData(*steps, *values2); // Устанавливаем данные
    customPlot->replot();           // Отрисовываем график
}
