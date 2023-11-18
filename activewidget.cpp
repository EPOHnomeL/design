#include "activewidget.h"
#include "mainwindow.h"
#include <QDebug>

ActiveWidget::ActiveWidget(QString acomPort, QWidget *parent) : QWidget(parent), ui(new Ui::ActiveWidget)
{
    ui->setupUi(this);
    connect(ui->b_disconnect, SIGNAL(clicked()), this, SLOT(disconnect()));
    comPort = acomPort;
    lcd_time = ui->lcd_time;
    lcd_armAngle = ui->lcd_armAnge;
    chartLayout = ui->chartLayout;

    series1 = new QLineSeries();
    series2 = new QLineSeries();
    series1->setName("Motor RPM");
    series2->setName("Bucket RPM");

    chart = new QChart();
    chart->setGeometry(0, 0, 500, 250);
    chart->legend()->hide();
    chart->addSeries(series1);
    chart->addSeries(series2);
    chart->setTitle("Motor and bucket RPM vs Time");
    chart->legend()->show();

    axisX = new QDateTimeAxis();
    axisX->setTickCount(5);
    axisX->setFormat("hh:mm:ss");
    chart->addAxis(axisX, Qt::AlignBottom);
    series1->attachAxis(axisX);
    series2->attachAxis(axisX);

    for(int i=0;i<1000;i++){
        series1->append(QDateTime::currentDateTime().toMSecsSinceEpoch()-10000+10*i, 0);
        series2->append(QDateTime::currentDateTime().toMSecsSinceEpoch()-10000+10*i, 0);
    }

    axisY = new QValueAxis;
    chart->addAxis(axisY, Qt::AlignLeft);
    series1->attachAxis(axisY);
    series2->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartLayout->addWidget(chartView);


    serial = MainWindow::findMainWindow()->getSerialPort();
    connect(serial, SIGNAL(motorRPMChanged(uint16_t)), this, SLOT(updateMotor(uint16_t)));
    connect(serial, SIGNAL(bucketRPMChanged(uint16_t)), this, SLOT(updateBucket(uint16_t)));
    connect(serial, SIGNAL(armAngleChanged(uint16_t)), this, SLOT(updateArmAngle(uint16_t)));
    connect(serial, SIGNAL(timeChanged(uint16_t)), this, SLOT(updateTime(uint16_t)));

    updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateChart()));
    updateTimer->start(10);
}

ActiveWidget::~ActiveWidget()
{


}

void ActiveWidget::updateBucket(uint16_t v)
{
        b = v;
}

void ActiveWidget::updateArmAngle(uint16_t v)
{
    lcd_armAngle->display(v*1.0f);
}

void ActiveWidget::updateTime(uint16_t v)
{
    lcd_time->display(m/10.0f);
}

void ActiveWidget::disconnect()
{

}

void ActiveWidget::updateMotor(uint16_t v)
{
    m = v;
}

void ActiveWidget::updateChart()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    series1->append(currentTime.toMSecsSinceEpoch(), m);
    series2->append(currentTime.toMSecsSinceEpoch(), b);

    while (series1->count() > 1000)
    {
        series1->remove(0);
        series2->remove(0);
    }
    axisX->setRange(QDateTime::fromMSecsSinceEpoch(series1->at(0).x()),
                    QDateTime::fromMSecsSinceEpoch(series1->at(series1->count() - 1).x()));
    axisY->setRange(0, 1000);
    chart->update();
}
