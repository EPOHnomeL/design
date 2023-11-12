#include "activewidget.h"

ActiveWidget::ActiveWidget(QString acomPort, QWidget *parent) : QWidget(parent), ui(new Ui::ActiveWidget)
{
    ui->setupUi(this);
    comPort = acomPort;

    stopButton = ui->stopButton;
    speedLCD = ui->speedLCD;
    timeLCD = ui->timeLCD;
    chartLayout = ui->chartLayout;

    QLineSeries* series = new QLineSeries();
    *series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);
    QChart *chart = new QChart();
    chart->setGeometry(0, 0, 500, 250);
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Speed vs Time");
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartLayout->addWidget(chartView);
}

ActiveWidget::~ActiveWidget()
{

}
