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
    reg.insert(QModbusDataUnit::HoldingRegisters, { QModbusDataUnit::HoldingRegisters, 0x0000, 40 });
    reg.insert(QModbusDataUnit::Coils, {QModbusDataUnit::Coils, 0x0030, 40});
    modbusDevice->setMap(reg);
    modbusDevice->connectDevice();
}

void Serial::readRegister(QModbusDataUnit::RegisterType table, int, int) {
    if (table != QModbusDataUnit::HoldingRegisters)
        return;


    QModbusDataUnit state = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0x0000, 1);
    if (modbusDevice->data(&state)) {
        emit stateChanged(state.value(0));
    }

    QModbusDataUnit currentProfile = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0x0001, 1);
    if (modbusDevice->data(&currentProfile)) {
        emit currentProfileChanged(currentProfile.value(0));
    }

    QModbusDataUnit motorRPM = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0x0002, 1);
    if (modbusDevice->data(&motorRPM)) {
        emit motorRPMChanged(motorRPM.value(0));
    }

    QModbusDataUnit bucketRPM = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0x0003, 1);
    if (modbusDevice->data(&bucketRPM)) {
        emit bucketRPMChanged(bucketRPM.value(0));
    }

    QModbusDataUnit armAngle = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0x0004, 1);
    if (modbusDevice->data(&armAngle)) {
        emit armAngleChanged(armAngle.value(0));
    }

    QModbusDataUnit time = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0x0005, 1);
    if (modbusDevice->data(&time)) {
        emit timeChanged(time.value(0));
    }

    QModbusDataUnit profile1 = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0x0010, 6);
    QModbusDataUnit profile1Rotations = QModbusDataUnit(QModbusDataUnit::Coils, 0x0010, 2);

    QModbusDataUnit profile2 = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0x0020, 6);
    QModbusDataUnit profile2Rotations = QModbusDataUnit(QModbusDataUnit::Coils, 0x0020, 2);

    QModbusDataUnit profile3 = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0x0030, 6);
    QModbusDataUnit profile3Rotations = QModbusDataUnit(QModbusDataUnit::Coils, 0x0030, 2);

    if (modbusDevice->data(&profile1) || modbusDevice->data(&profile1Rotations)){
        QList<quint16> result;
        result.append(profile1.values());
        result.append(profile1Rotations.values());
        emit profile1Changed(result);
    }

    if (modbusDevice->data(&profile2) || modbusDevice->data(&profile2Rotations)){
        QList<quint16> result;
        result.append(profile2.values());
        result.append(profile2Rotations.values());
        emit profile2Changed(result);
    }

    if (modbusDevice->data(&profile3) || modbusDevice->data(&profile3Rotations)){
        QList<quint16> result;
        result.append(profile3.values());
        result.append(profile3Rotations.values());
        emit profile3Changed(result);
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
