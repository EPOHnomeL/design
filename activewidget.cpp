#include "activewidget.h"
#include "mainwindow.h"
#include <QDebug>

ActiveWidget::ActiveWidget(QString acomPort, QWidget *parent) : QWidget(parent), ui(new Ui::ActiveWidget)
{
    ui->setupUi(this);
    comPort = acomPort;
    speedLCD = ui->speedLCD;
    timeLCD = ui->timeLCD;
    chartLayout = ui->chartLayout;

    series = new QLineSeries();
    chart = new QChart();
    chart->setGeometry(0, 0, 500, 250);
    chart->legend()->hide();
    chart->addSeries(series);
    chart->setTitle("Speed vs Time");

    axisX = new QDateTimeAxis();
    axisX->setTickCount(5);
    axisX->setFormat("hh:mm:ss");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    for(int i=0;i<1000;i++){
        series->append(QDateTime::currentDateTime().toMSecsSinceEpoch()-10000+10*i, 0);
    }

    axisY = new QValueAxis;
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartLayout->addWidget(chartView);


    serial = MainWindow::findMainWindow()->getSerialPort();
    connect(serial, SIGNAL(currentSpeedChanged(uint16_t)), this, SLOT(recieveValue(uint16_t)));

    updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateChart()));
    updateTimer->start(10);
}

ActiveWidget::~ActiveWidget()
{

}

void ActiveWidget::recieveValue(uint16_t v)
{
    value = v*200/1024;
}

void ActiveWidget::updateChart()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    series->append(currentTime.toMSecsSinceEpoch(), value);
    speedLCD->display(value);

    // TODO: Set time to correct values //
    time = time - 0.001f;
    timeLCD->display(time);

    while (series->count() > 1000)
    {
        series->remove(0);
    }
    axisX->setRange(QDateTime::fromMSecsSinceEpoch(series->at(0).x()),
                    QDateTime::fromMSecsSinceEpoch(series->at(series->count() - 1).x()));
    axisY->setRange(0, 200);
    chart->update();
}
