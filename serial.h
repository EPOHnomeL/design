#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QModbusDataUnit>
#include <QModbusServer>
#include <QModbusRtuSerialMaster>
#include <QModbusRtuSerialSlave>
#include <QSerialPortInfo>
#include <QDebug>

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

private:
    const QString END_MESSAGE = "!";
    QSerialPort serialPort;
    QSerialPortInfo info;
    QString buffer;

    QModbusReply *reply;
//    QModbusRtuSerialMaster modbusMaster;
    QModbusRtuSerialSlave modbusSlave;
    void SetupModbus(QString);
};

#endif // SERIAL_H
