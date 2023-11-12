#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

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

private:
    const QString END_MESSAGE = "!";
    QSerialPort serialPort;
    QSerialPortInfo info;
    QString buffer;
};

#endif // SERIAL_H
