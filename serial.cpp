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

    QModbusDataUnit profiles = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0x00010, 40);
    QModbusDataUnit profilesRotations = QModbusDataUnit(QModbusDataUnit::Coils, 0x00010, 40);
    if (modbusDevice->data(&profiles) || modbusDevice->data(&profilesRotations)) {
        QList<quint16> result;
        QList<quint16> profileData = profiles.values();
        QList<quint16> profilesRotationsData = profilesRotations.values();
        for(int i=0;i<3;i++){
            for(int j=0;j<10;j++){
                if(j<=5)
                    result.append(profileData.at(i*10+j));
                if(j>=6&& j<=7){
                    result.append(profilesRotationsData.at(i*10+(j-6)));
                }
            }
        }

        emit profilesChanged(result);
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
