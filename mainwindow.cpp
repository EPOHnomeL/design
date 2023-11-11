#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <stdio.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this,SIGNAL(messageFinished()),this,SLOT(messageReceived()));
    tabWidget = ui->tabWidget;
    connect(tabWidget, SIGNAL(currentChanged(int)), this, (SLOT(tabchange(int))));

    start = new StartWidget();

    int currentIndex = ui->tabWidget->currentIndex();
    ui->tabWidget->removeTab(currentIndex);
    ui->tabWidget->insertTab(currentIndex, start, "Mixer 1");
    ui->tabWidget->setCurrentIndex(currentIndex);
//     vLayoutRPM = ui->vLayoutRPM;
//     QLineSeries* series = new QLineSeries();
//     *series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);
//     QChart *chart = new QChart();
//     chart->legend()->hide();
//     chart->addSeries(series);
//     chart->createDefaultAxes();
//     chart->setTitle("Simple line chart example");
//     QChartView *chartView = new QChartView(chart);
//     chartView->setRenderHint(QPainter::Antialiasing);
//     vLayoutRPM->addWidget(chartView);

    // Ports
//    QList<QSerialPortInfo> ports = info.availablePorts();
//    foreach(auto &x,ports)
//        qDebug()<<x.portName();
//    if(ports.length() == 0){
//        qDebug() << "Please Insert RS485 usb stick!";
//        exit(EXIT_FAILURE);
//    }
//    setupSerial(ports[1].portName(), 256000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::messageStream()
{
    QByteArray dataBA = serialPort.readAll();
    QString data(dataBA);
    if(data == ""){
        return;
    }
    for(int i=0; i<data.length();i++){
        if(data[i] == END_MESSAGE){
            emit messageFinished();
        }else{
            buffer = buffer +  data[i];
        }
    }
}

void MainWindow::setupSerial(QString portName, int baudrate){
    serialPort.setPortName(portName);
    serialPort.open(QIODevice::ReadWrite);
    if(!serialPort.isOpen()){
        qDebug() << "!!!! Something went Wrong !!!!";
        exit(EXIT_FAILURE);
    }
    serialPort.setBaudRate(baudrate);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setStopBits(QSerialPort::OneStop);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setFlowControl(QSerialPort::NoFlowControl);
    qDebug() << "Connected";
    connect(&serialPort,SIGNAL(readyRead()),this,SLOT(messageStream()));
}

void MainWindow::messageReceived()
{
    qDebug() << buffer;
    buffer = "";
}

void MainWindow::tabchange(int i)
{
//    tabCount++;
//    tabWidget->insertTab(tabCount-1, )
    qDebug() << i;
}

