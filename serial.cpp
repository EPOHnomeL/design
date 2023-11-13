#include "serial.h"
#include <QDebug>
#include <QModbusRtuSerialClient>

Serial::Serial(QString portName, int baudrate, QObject *parent) : QObject(parent)
{
    connect(this,SIGNAL(MessageFinished(QString)),this,SLOT(MessageReceived(QString)));
//    QModbusRtuSerialClient  *client = new QModbusClient();
//    client->setConnectionParameter(QModbusDevice::SerialPortNameParameter, portName);
//    client->setConnectionParameter(QModbusDevice::SerialParityParameter, QSerialPort::NoParity);
//    client->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, QSerialPort::Baud115200);
//    client->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, QSerialPort::Data8);
//    client->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, QSerialPort::OneStop);
//    client->setConnectionParameter(QModbusDevice::NetworkAddressParameter, 1);


////    SetupSerial(portName, 9600);
////    SetupModbus(portName);
//    QModbusRtuSerialSlave* modbusServer = new QModbusRtuSerialSlave();
//    modbusServer->setConnectionParameter(QModbusDevice::SerialPortNameParameter, portName);
//    modbusServer->setConnectionParameter(QModbusDevice::SerialParityParameter, QSerialPort::NoParity);
//    modbusServer->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, QSerialPort::Baud115200);
//    modbusServer->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, QSerialPort::Data8);
//    modbusServer->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, QSerialPort::OneStop);
//    modbusServer->setServerAddress(1);

//    QModbusDataUnit unit(QModbusDataUnit::HoldingRegisters, 0x0000, 29); // Type, Address, Size
//    modbusServer->setData(unit);


//    // Connect the dataWritten signal to a slot
//    QObject::connect(modbusServer, &QModbusServer::dataWritten, [=](QModbusDataUnit::RegisterType registerType, int address, int size) {
//            qDebug() << "Data written to Holding Registers" << 0x0000 << "and" << 0x0000 + 1 << ":"
//                     << unit.value(0) + (unit.value(1) << 8); // Combine the two registers to get the original analog value

//    });
//    if (!modbusServer->connectDevice()) {
//        qDebug() << "Could not connect to serial port";
//        exit(EXIT_FAILURE);
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
    qDebug() << "Analog Value:" << reply->result().values();
}

const QSerialPort &Serial::getSerialPort() const
{
    return serialPort;
}

void Serial::disconnect()
{
//        serialPort.close();
}
