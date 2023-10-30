#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void messageFinished();

private slots:
    void messageStream();
    void setupSerial(QString portName, int baudrate);
    void messageReceived();


private:
    const QString END_MESSAGE = "!";
    Ui::MainWindow *ui;
    QSerialPort serialPort;
    QSerialPortInfo info;
    QString buffer;
};
#endif // MAINWINDOW_H
