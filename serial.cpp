#include "serial.h"
#include <QDebug>
#include <QVariant>
#include <QModbusReply>

Serial::Serial(QString portName, int baudrate, QObject *parent) : QObject(parent)
{
    setupModbusDevice();
}

void Serial::setupModbusDevice() {
    modbusDevice = new QModbusRtuSerialServer();
    modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter, "COM3");
    modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, QSerialPort::Baud9600);
    modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, QSerialPort::Data8);
    modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter, QSerialPort::NoParity);
    modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, QSerialPort::OneStop);
    modbusDevice->setServerAddress(1);
    QModbusDataUnitMap reg;
    reg.insert(QModbusDataUnit::HoldingRegisters, { QModbusDataUnit::HoldingRegisters, 0, 6 });
    modbusDevice->setMap(reg);

    modbusDevice->connectDevice();
    connect(modbusDevice, SIGNAL(dataWritten(QModbusDataUnit::RegisterType,int,int)),
            this, SLOT(readRegister(QModbusDataUnit::RegisterType,int,int)));
}

void Serial::readRegister(QModbusDataUnit::RegisterType table, int address, int size) {
    if (table == QModbusDataUnit::HoldingRegisters && address == 0 && size == 6) {
        QModbusDataUnit req = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0, 6);
        if (modbusDevice->data(&req)) {
            for (uint i = 0; i < req.valueCount(); i++) {
                qDebug() << "Value at register" << i << "is" << req.value(i);
            }
        } else {
            qDebug() << "Read error: " << modbusDevice->errorString();
        }
    }
}
//    readRegister();
//}

//void Serial::readRegister() {
//    QModbusDataUnit req = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 0, 6);
//    if (modbusDevice->data(&req)) {
//        for (uint i = 0; i < req.valueCount(); i++) {
//            qDebug() << "Value at register" << i << "is" << req.value(i);
//        }
//    } else {
//        qDebug() << "Read error: " << modbusDevice->errorString();
//    }
//}



//    QModbusRtuSerialServer* modbusServer = new QModbusRtuSerialServer();
//    modbusServer->setConnectionParameter(QModbusDevice::SerialPortNameParameter, portName);
//    modbusServer->setConnectionParameter(QModbusDevice::SerialParityParameter, QSerialPort::NoParity);
//    modbusServer->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, QSerialPort::Baud115200);
//    modbusServer->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, QSerialPort::Data8);
//    modbusServer->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, QSerialPort::OneStop);
//    modbusServer->setServerAddress(1);

//    QModbusDataUnit unit(QModbusDataUnit::HoldingRegisters, 0x40000, 16); // Type, Address, Size
//    modbusServer->setData(unit);


//    // Connect the dataWritten signal to a slot
//    QObject::connect(modbusServer, &QModbusRtuSerialServer::dataWritten, [=](QModbusDataUnit::RegisterType registerType, int address, int size) {
//            qDebug() << "Data written to Holding Registers" << 0x0000 << "and" << 0x0000 + 1 << ":"
//                     << unit.value(0) + (unit.value(1) << 8); // Combine the two registers to get the original analog value

//    });
//    if (!modbusServer->connectDevice()) {
//        qDebug() << "Could not connect to serial port";
//        exit(EXIT_FAILURE);
//    } else {
//        qDebug() << "Connected";
////        unit.
//    }
//}

//    QModbusRtuSerialServer server;
//    connect(this,SIGNAL(MessageFinished(QString)),this,SLOT(MessageReceived(QString)));
//    connect(&server, SIGNAL(dataWritten(QModbusDataUnit::RegisterType, int, int)), this, SLOT(read(QModbusDataUnit::RegisterType, int, int)));


//    // Set the parameters for the serial port
//    server.setConnectionParameter(QModbusDevice::SerialPortNameParameter, portName);
//    server.setConnectionParameter(QModbusDevice::SerialParityParameter, QSerialPort::NoParity);
//    server.setConnectionParameter(QModbusDevice::SerialBaudRateParameter, QSerialPort::Baud115200);
//    server.setConnectionParameter(QModbusDevice::SerialDataBitsParameter, QSerialPort::Data8);
//    server.setConnectionParameter(QModbusDevice::SerialStopBitsParameter, QSerialPort::OneStop);
//    server.setServerAddress(1);

//    if (!server.connectDevice()) {
//        qDebug() << "Connection failed:" << server.errorString();
//    }
//}

//void Serial::SetupModbus(QString portName)
//{
//    modbusMaster.setConnectionParameter(QModbusDevice::SerialPortNameParameter, portName);
//    modbusMaster.setConnectionParameter(QModbusDevice::SerialParityParameter, QSerialPort::NoParity);
//    modbusMaster.setConnectionParameter(QModbusDevice::SerialBaudRateParameter, QSerialPort::Baud9600);
//    modbusMaster.setConnectionParameter(QModbusDevice::SerialDataBitsParameter, QSerialPort::Data8);
//    modbusMaster.setConnectionParameter(QModbusDevice::SerialStopBitsParameter, QSerialPort::OneStop);
//    if (!modbusMaster.connectDevice()) {
//        qDebug() << "Connection failed:" << modbusMaster.errorString();
//        exit(EXIT_FAILURE);
//    }

//    QModbusDataUnit readUnit(QModbusDataUnit::HoldingRegisters, 0, 1);
//    reply = modbusMaster.sendReadRequest(readUnit, 11);

////    QModbusRequest req(QModbusPdu::ReadHoldingRegisters);
////    reply = modbusMaster.sendRawRequest(req, 1);

//    if (!reply) {
//        qDebug() << "Read request error:" << modbusMaster.errorString();
//        exit(EXIT_FAILURE);
//    }

//    QObject::connect(reply, SIGNAL(finished()), this, SLOT(finished()));
//}

Serial::~Serial()
{
    serialPort.close();
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
            emit MessageFinished(buffer);
            buffer = "";
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
    connect(&serialPort,SIGNAL(readyRead()),this,SLOT(MessageStream()));
}

void Serial::MessageReceived(QString)
{
    qDebug() << buffer;
}

void Serial::finished()
{
    //    qDebug() << "Analog Value:" << reply->result().values();
}


const QSerialPort &Serial::getSerialPort() const
{
    return serialPort;
}

void Serial::disconnect()
{
//        serialPort.close();
}
