#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSerialPortInfo>
#include <QDebug>
#include <QModbusDataUnit>
#include <QModbusRtuSerialServer>

class Serial : public QObject
{
    Q_OBJECT
public:
    explicit Serial(QString portName, int baudrate, QObject *parent = nullptr);
    ~Serial();
    const QSerialPort &getSerialPort() const;
    void disconnect();

signals:
    void MessageFinished(QString);

private slots:
    void MessageStream();
    void SetupSerial(QString portName, int baudrate);
    void MessageReceived(QString);
    void finished();
    void readRegister(QModbusDataUnit::RegisterType table, int address, int size);

private:
    const QString END_MESSAGE = "!";
    QSerialPort serialPort;
    QSerialPortInfo info;
    QString buffer;
    void setupModbusDevice();
    void readRegister();
//    QModbusReply *reply;
//    QModbusRtuSerialMaster modbusMaster;
//    QModbusRtuSerialSlave modbusSlave;
    void SetupModbus(QString);
    QModbusRtuSerialServer *modbusDevice;
};

#endif // SERIAL_H
