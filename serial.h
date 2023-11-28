#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSerialPortInfo>
#include <QDebug>
#include <QModbusDataUnit>
#include <QModbusRtuSerialServer>
#include <QElapsedTimer>

class Serial : public QObject
{
    Q_OBJECT
public:
    explicit Serial(QString portName, QObject *parent = nullptr);
    ~Serial();
    void disconnect();
    QString getPortName() const;
    QElapsedTimer *getTimer();

signals:
    void stateChanged(uint16_t);
    void currentProfileChanged(uint16_t);
    void motorRPMChanged(uint16_t);
    void bucketRPMChanged(uint16_t);
    void rotationsChanged(QList<uint16_t>);
    void armAngleChanged(uint16_t);
    void timeChanged(uint16_t);
    void profile1Changed(QList<quint16>);
    void profile2Changed(QList<quint16>);
    void profile3Changed(QList<quint16>);

private slots:
    void readRegister(QModbusDataUnit::RegisterType table, int address, int size);

private:
    QElapsedTimer *timer;
    QString portName;
    void setupModbusDevice(QString);
    void readRegister();
    void SetupModbus(QString);
    QModbusRtuSerialServer *modbusDevice;
};

#endif // SERIAL_H
