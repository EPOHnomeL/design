#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <stdio.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this,SIGNAL(MessageFinished()),this,SLOT(MessageReceived()));
    tabWidget = ui->tabWidget;
    connect(tabWidget, SIGNAL(currentChanged(int)), this, (SLOT(TabChange(int))));

    availablePorts = info.availablePorts();

    startWidgets[0] = new StartWidget();
    connect(startWidgets[0], SIGNAL(ComPortSelected(QString)), this, SLOT(InitMixer(QString)));


    int currentIndex = ui->tabWidget->currentIndex();
    ui->tabWidget->removeTab(currentIndex);
    ui->tabWidget->insertTab(currentIndex, startWidgets[0], "Mixer 1");
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

QList<QSerialPortInfo> MainWindow::getAvailablePorts()
{
    return availablePorts;
}

void MainWindow::MessageStream()
{
    QByteArray dataBA = serialPort.readAll();
    QString data(dataBA);
    if(data == ""){
        return;
    }
    for(int i=0; i<data.length();i++){
        if(data[i] == END_MESSAGE){
            emit MessageFinished();
        }else{
            buffer = buffer +  data[i];
        }
    }
}

void MainWindow::SetupSerial(QString portName, int baudrate){
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

void MainWindow::MessageReceived()
{
    qDebug() << buffer;
    buffer = "";
}

void MainWindow::TabChange(int i)
{
    if(i == tabCount){
        if(tabCount != MAX_TABS){
            startWidgets[tabCount] = new StartWidget();
            connect(startWidgets[tabCount], SIGNAL(ComPortSelected(QString)), this, SLOT(InitMixer(QString)));
            tabWidget->insertTab(tabCount, startWidgets[tabCount], QString("Mixer %1").arg(tabCount+1));
            tabCount++;
            tabWidget->setCurrentIndex(tabCount-1);
        } else {
            tabWidget->setCurrentIndex(currentTab);
        }
    } else{
        currentTab = i;
    }
}

void MainWindow::InitMixer(QString comPort)
{
    for(int i=0;i<availablePorts.size();i++){
        if(availablePorts[i].portName() == comPort){
            availablePorts.removeAt(i);
        }
    }
    initWidgets[currentTab] = new InitWidget(comPort);
    connect(initWidgets[currentTab], SIGNAL(Start()), this, SLOT(Start()));
    switchWidgets(initWidgets[currentTab]);
}

void MainWindow::Start()
{

}

void MainWindow::switchWidgets(QWidget *w)
{
    ui->tabWidget->removeTab(currentTab);
    ui->tabWidget->insertTab(currentTab, w,  QString("Mixer %1").arg(currentTab+1));
    ui->tabWidget->setCurrentIndex(currentTab);
}

MainWindow* MainWindow::findMainWindow()
{
    for(QWidget* pWidget : QApplication::topLevelWidgets())
    {
        MainWindow *pMainWnd = qobject_cast<MainWindow*>(pWidget);
        if (pMainWnd)
           return pMainWnd;
    }
    return nullptr;
}

