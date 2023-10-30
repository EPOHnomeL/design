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
    // Ports
    QList<QSerialPortInfo> ports = info.availablePorts();
    foreach(auto &x,ports)
        qDebug()<<x.portName();
    if(ports.length() == 0){
        qDebug() << "Please Insert RS485 usb stick!";
        exit(EXIT_FAILURE);
    }
    setupSerial(ports[1].portName(), 256000);
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

