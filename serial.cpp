#include "serial.h"
#include <QDebug>
#include <QVariant>
#include <QModbusReply>

Serial::Serial(QString aportName, QObject *parent) : QObject(parent)
{
    portName = aportName;
    setupModbusDevice(portName);
    connect(modbusDevice, SIGNAL(dataWritten(QModbusDataUnit::RegisterType,int,int)),
            this, SLOT(readRegister(QModbusDataUnit::RegisterType,int,int)));
}

void Serial::setupModbusDevice(QString portName) {
    modbusDevice = new QModbusRtuSerialServer();
    modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter, portName);
    modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, QSerialPort::Baud115200);
    modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, QSerialPort::Data8);
    modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter, QSerialPort::NoParity);
    modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, QSerialPort::OneStop);
    modbusDevice->setServerAddress(1);

    QModbusDataUnitMap reg;
    reg.insert(QModbusDataUnit::HoldingRegisters, { QModbusDataUnit::HoldingRegisters, 0, 3 });
    modbusDevice->setMap(reg);
    modbusDevice->connectDevice();
}

void Serial::readRegister(QModbusDataUnit::RegisterType, int, int) {
    QModbusDataUnit req = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0, 3);
    if (modbusDevice->data(&req)) {
        qDebug() << req.values();
        emit statusChanged(req.value(1));
        emit profilesChanged(req.value(2));
        emit currentSpeedChanged(req.value(0));
    } else {
        qDebug() << "Read error: " << modbusDevice->errorString();
    }
}

QString Serial::getPortName() const
{
    return portName;
}

Serial::~Serial()
{
    modbusDevice->disconnectDevice();
}


void Serial::disconnect()
{
    modbusDevice->disconnectDevice();
}
