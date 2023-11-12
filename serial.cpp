#include "serial.h"
#include <QDebug>

Serial::Serial(QString portName, int baudrate, QObject *parent) : QObject(parent)
{
    connect(this,SIGNAL(MessageFinished()),this,SLOT(MessageReceived()));
    SetupSerial(portName, baudrate);
}

void Serial::MessageStream()
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

void Serial::SetupSerial(QString portName, int baudrate){
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

void Serial::MessageReceived()
{
    qDebug() << buffer;
    buffer = "";
}
