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
    reg.insert(QModbusDataUnit::HoldingRegisters, { QModbusDataUnit::HoldingRegisters, 0x0000, 1 });
    reg.insert(QModbusDataUnit::HoldingRegisters, { QModbusDataUnit::HoldingRegisters, 0x0001, 1 });
    reg.insert(QModbusDataUnit::HoldingRegisters, { QModbusDataUnit::HoldingRegisters, 0x0002, 1 });
    reg.insert(QModbusDataUnit::HoldingRegisters, { QModbusDataUnit::HoldingRegisters, 0x0003, 1 });
    reg.insert(QModbusDataUnit::HoldingRegisters, { QModbusDataUnit::HoldingRegisters, 0x0004, 12 });
    modbusDevice->setMap(reg);
    modbusDevice->connectDevice();
}

void Serial::readRegister(QModbusDataUnit::RegisterType table, int, int) {
    if (table != QModbusDataUnit::HoldingRegisters)
        return;


    QModbusDataUnit status = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0x0000, 1);
    if (modbusDevice->data(&status)) {
        emit statusChanged(status.value(0));
    }

    QModbusDataUnit rpm = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0x0001, 1);
    if (modbusDevice->data(&rpm)) {
        emit rpmChanged(rpm.value(0));
    }

    QModbusDataUnit time = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0x0002, 1);
    if (modbusDevice->data(&time)) {
        emit timeChanged(time.value(0));
    }

    QModbusDataUnit currentProfile = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0x0003, 1);
    if (modbusDevice->data(&currentProfile)) {
        emit currentProfileChanged(currentProfile.value(0));
    }

    QModbusDataUnit profiles = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0x0004, 12);
    if (modbusDevice->data(&profiles)) {
        emit profilesChanged(profiles.values());
    }
}

//void Serial::readRegister(QModbusDataUnit::RegisterType, int, int) {
//    QModbusDataUnit req = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0, 3);
//    if (modbusDevice->data(&req)) {
//        qDebug() << req.values();
//        emit statusChanged(req.value(1));
//        emit profilesChanged(req.value(2));
//        emit currentSpeedChanged(req.value(0));
//    } else {
//        qDebug() << "Read error: " << modbusDevice->errorString();
//    }
//}

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
