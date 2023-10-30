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
    void messageRecieved();

private slots:
    void messageStream();
    void setupSerial(QString portName, int baudrate);


private:
    Ui::MainWindow *ui;
    QSerialPort serialPort;
    QSerialPortInfo info;
    QString buffer;
};
#endif // MAINWINDOW_H
