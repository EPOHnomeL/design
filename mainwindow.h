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
#include "initwidget.h"

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
    void init(QString);

private:
    const static int MAX_TABS = 5;
    const QString END_MESSAGE = "!";
    Ui::MainWindow *ui;
    int tabCount = 1;
    int currentTab = 0;
    QSerialPort serialPort;
    QSerialPortInfo info;
    QString buffer;
    QTabWidget *tabWidget;

    StartWidget *startWidgets[MAX_TABS];
    InitWidget *initWidgets[MAX_TABS];

};
#endif // MAINWINDOW_H
