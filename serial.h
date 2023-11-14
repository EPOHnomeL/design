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
    explicit Serial(QString portName, QObject *parent = nullptr);
    ~Serial();
    void disconnect();
    QString getPortName() const;

signals:
    void statusChanged(uint16_t);
    void rpmChanged(uint16_t);
    void timeChanged(uint16_t);
    void profilesChanged(QList<quint16>);
    void currentProfileChanged(uint16_t);
    void currentSpeedChanged(uint16_t);

private slots:
    void readRegister(QModbusDataUnit::RegisterType table, int address, int size);

private:
    QString portName;
    void setupModbusDevice(QString);
    void readRegister();
    void SetupModbus(QString);
    QModbusRtuSerialServer *modbusDevice;
};

#endif // SERIAL_H
