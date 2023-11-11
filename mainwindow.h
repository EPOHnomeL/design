#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QtCharts>
using namespace QtCharts;
#include <QLineSeries>
#include <QChartView>
#include "startwidget.h"

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
    void tabchange(int);


private:
    const QString END_MESSAGE = "!";
    Ui::MainWindow *ui;
    int tabCount = 1;
    QSerialPort serialPort;
    QSerialPortInfo info;
    QString buffer;
    QTabWidget *tabWidget;
    StartWidget *start;
};
#endif // MAINWINDOW_H
