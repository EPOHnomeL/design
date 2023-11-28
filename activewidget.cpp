#include "activewidget.h"
#include "mainwindow.h"
#include <QDebug>

ActiveWidget::ActiveWidget(QString acomPort, QWidget *parent) : QWidget(parent), ui(new Ui::ActiveWidget)
{
    ui->setupUi(this);
    connect(ui->b_disconnect, SIGNAL(clicked()), this, SLOT(disconnectThis()));
    comPort = acomPort;
    img_mr = ui->img_mr;
    img_br = ui->img_br;

    QPixmap br(":/rot/Anti-Clockwise.jpg");
    QPixmap scaledPixmap = br.scaledToHeight(img_mr->maximumHeight(), Qt::SmoothTransformation);
    ui->img_br->setPixmap(scaledPixmap);
    ui->img_mr->setPixmap(scaledPixmap);

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


    updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateChart()));
}

ActiveWidget::~ActiveWidget()
{


}

void ActiveWidget::start()
{
    updateTimer->start(10);
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
    lcd_time->display(v/10.0f);
}

void ActiveWidget::disconnectThis()
{
    updateTimer->stop();
    emit disconnectActive(comPort);
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

void ActiveWidget::rotationsChanged(QList<uint16_t> r)
{
    QPixmap br(QString(":/rot/%1.jpg").arg(r.at(0) ? "Anti-Clockwise" : "Clockwise"));
    QPixmap scaledPixmap1 = br.scaledToHeight(img_br->maximumHeight(), Qt::SmoothTransformation);
    ui->img_br->setPixmap(scaledPixmap1);

    QPixmap mr(QString(":/rot/%1.jpg").arg(r.at(1) ? "Anti-Clockwise" : "Clockwise"));
    QPixmap scaledPixmap2 = mr.scaledToHeight(img_mr->maximumHeight(), Qt::SmoothTransformation);
    ui->img_mr->setPixmap(scaledPixmap2);
}
