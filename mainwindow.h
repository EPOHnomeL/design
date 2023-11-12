#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "startwidget.h"
#include "initwidget.h"
#include "activewidget.h"
#include "serial.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QList<QString> getAvailablePorts();
    Serial *getSerialPort();
    static MainWindow* findMainWindow();


private slots:
    void TabChange(int);
    void InitMixer(QString);
    void Start(QString);
    void Disconnect(QString);

private:
    const static int MAX_TABS = 5;
    Ui::MainWindow *ui;
    int tabCount = 1;
    int currentTab = 0;
    QTabWidget *tabWidget;
    QList<QString> availablePorts;

    Serial *serials[MAX_TABS];
    StartWidget *startWidgets[MAX_TABS];
    InitWidget *initWidgets[MAX_TABS];
    ActiveWidget *activeWidgets[MAX_TABS];

    QSerialPortInfo info;
    void switchWidgets(QWidget *w, QString name ="");

};
#endif // MAINWINDOW_H
