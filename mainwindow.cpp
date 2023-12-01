#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <stdio.h>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tabWidget = ui->tabWidget;
    connect(tabWidget, SIGNAL(currentChanged(int)), this, (SLOT(TabChange(int))));

    QList<QSerialPortInfo> allPorts = info.availablePorts();
    for(int i=0; i<allPorts.size();i++){
        availablePorts.append(allPorts[i].portName());
    }

    for(int j=0;j<5;j++){
        ports[j] = "";
    }

    startWidgets[0] = new StartWidget();
    connect(startWidgets[0], SIGNAL(ComPortSelected(QString)), this, SLOT(InitMixer(QString)));

    int currentIndex = ui->tabWidget->currentIndex();
    ui->tabWidget->removeTab(currentIndex);
    ui->tabWidget->insertTab(currentIndex, startWidgets[0], "Mixer 1");
    ui->tabWidget->setCurrentIndex(currentIndex);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QList<QString> MainWindow::getAvailablePorts()
{
    return availablePorts;
}

Serial *MainWindow::getSerialPort()
{
    return serials[currentTab];
}

void MainWindow::TabChange(int i)
{
    if(i == tabCount){
        if(tabCount != MAX_TABS){
            startWidgets[tabCount] = new StartWidget();
            connect(startWidgets[tabCount], SIGNAL(ComPortSelected(QString)), this, SLOT(InitMixer(QString)));
            tabWidget->insertTab(tabCount, startWidgets[tabCount], QString("Mixer %1").arg(tabCount+1));
            tabCount++;
            tabWidget->setCurrentIndex(tabCount-1);
        } else {
            tabWidget->setCurrentIndex(currentTab);
        }
    } else{
        currentTab = i;
    }
}

void MainWindow::chosenPort(QString s){
    ports[currentTab] = s;
}

void MainWindow::Disconnect(QString portName)
{
    for(int i=0; i<MAX_TABS;i++){
        if(serials[i]){
            if(serials[i]->getPortName() == portName){
               serials[i]->disconnect();
               break;
            }
        }
    }
    availablePorts.append(portName);

    disconnect(serials[currentTab], SIGNAL(profile1Changed(QList<quint16>)), initWidgets[currentTab], SLOT(refreshProfile1(QList<quint16>)));
    disconnect(serials[currentTab], SIGNAL(profile2Changed(QList<quint16>)), initWidgets[currentTab], SLOT(refreshProfile2(QList<quint16>)));
    disconnect(serials[currentTab], SIGNAL(profile3Changed(QList<quint16>)), initWidgets[currentTab], SLOT(refreshProfile3(QList<quint16>)));
    disconnect(serials[currentTab], SIGNAL(currentProfileChanged(quint16)), initWidgets[currentTab], SLOT(currentProfileChanged(quint16)));
    disconnect(serials[currentTab], SIGNAL(stateChanged(uint16_t)), this, SLOT(statusChanged(uint16_t)));
    serials[currentTab]->disconnect();
    startWidgets[currentTab]->Reset();
    switchWidgets(startWidgets[currentTab]);
}

void MainWindow::disconnectActive(QString p)
{
    availablePorts.append(p);
    serials[currentTab]->disconnect();
    disconnect(serials[currentTab], SIGNAL(motorRPMChanged(uint16_t)), activeWidgets[currentTab], SLOT(updateMotor(uint16_t)));
    disconnect(serials[currentTab], SIGNAL(bucketRPMChanged(uint16_t)), activeWidgets[currentTab], SLOT(updateBucket(uint16_t)));
    disconnect(serials[currentTab], SIGNAL(armAngleChanged(uint16_t)), activeWidgets[currentTab], SLOT(updateArmAngle(uint16_t)));
    disconnect(serials[currentTab], SIGNAL(timeChanged(uint16_t)), activeWidgets[currentTab], SLOT(updateTime(uint16_t)));
    disconnect(serials[currentTab], SIGNAL(rotationsChanged(QList<uint16_t>)), activeWidgets[currentTab], SLOT(rotationsChanged(QList<uint16_t>)));
    disconnect(serials[currentTab], SIGNAL(profile1Changed(QList<quint16>)), initWidgets[currentTab], SLOT(refreshProfile1(QList<quint16>)));
    disconnect(serials[currentTab], SIGNAL(profile2Changed(QList<quint16>)), initWidgets[currentTab], SLOT(refreshProfile2(QList<quint16>)));
    disconnect(serials[currentTab], SIGNAL(profile3Changed(QList<quint16>)), initWidgets[currentTab], SLOT(refreshProfile3(QList<quint16>)));
    disconnect(serials[currentTab], SIGNAL(stateChanged(uint16_t)), this, SLOT(statusChanged(uint16_t)));
    startWidgets[currentTab]->Reset();
    switchWidgets(startWidgets[currentTab]);
}

void MainWindow::statusChanged(uint16_t state)
{
    if(!serials[currentTab]){
        return;
    }
    switch(state){
    case 3:
        if(!qobject_cast<InitWidget*>(tabWidget->currentWidget())) {
            switchWidgets(initWidgets[currentTab], ports[currentTab]);
        }
        break;
    case 4:
        if(!qobject_cast<ActiveWidget*>(tabWidget->currentWidget())) {
            if(first[currentTab]){
               first[currentTab] = !first[currentTab];
               activeWidgets[currentTab] = new ActiveWidget(ports[currentTab]);
            }
            connect(serials[currentTab], SIGNAL(motorRPMChanged(uint16_t)), activeWidgets[currentTab], SLOT(updateMotor(uint16_t)));
            connect(serials[currentTab], SIGNAL(bucketRPMChanged(uint16_t)), activeWidgets[currentTab], SLOT(updateBucket(uint16_t)));
            connect(serials[currentTab], SIGNAL(armAngleChanged(uint16_t)), activeWidgets[currentTab], SLOT(updateArmAngle(uint16_t)));
            connect(serials[currentTab], SIGNAL(rotationsChanged(QList<uint16_t>)), activeWidgets[currentTab], SLOT(rotationsChanged(QList<uint16_t>)));
            connect(serials[currentTab], SIGNAL(timeChanged(uint16_t)), activeWidgets[currentTab], SLOT(updateTime(uint16_t)));
            connect(activeWidgets[currentTab], SIGNAL(disconnectActive(QString)), this, SLOT(disconnectActive(QString)));
            activeWidgets[currentTab]->start();
            switchWidgets(activeWidgets[currentTab], ports[currentTab]);
        }
        break;
    case 5:
        // Show message check you machine
        disconnectActive(serials[currentTab]->getPortName());
        break;
    default:
        return;
    }
}

void MainWindow::InitMixer(QString port)
{
    ports[currentTab] = port;
    for(int i=0;i<availablePorts.size();i++){
        if(availablePorts[i] == port){
            availablePorts.removeAt(i);
        }
    }
    initWidgets[currentTab] = new InitWidget(port);
    serials[currentTab] = new Serial(port);
    connect(serials[currentTab], SIGNAL(profile1Changed(QList<quint16>)), initWidgets[currentTab], SLOT(refreshProfile1(QList<quint16>)));
    connect(serials[currentTab], SIGNAL(profile2Changed(QList<quint16>)), initWidgets[currentTab], SLOT(refreshProfile2(QList<quint16>)));
    connect(serials[currentTab], SIGNAL(profile3Changed(QList<quint16>)), initWidgets[currentTab], SLOT(refreshProfile3(QList<quint16>)));
    connect(serials[currentTab], SIGNAL(currentProfileChanged(quint16)), initWidgets[currentTab], SLOT(currentProfileChanged(quint16)));
    connect(serials[currentTab], SIGNAL(stateChanged(uint16_t)), this, SLOT(statusChanged(uint16_t)));
    connect(initWidgets[currentTab], SIGNAL(Disconnect(QString)), this, SLOT(Disconnect(QString)));
    switchWidgets(initWidgets[currentTab], port);
}

void MainWindow::switchWidgets(QWidget *w, QString name)
{
    ui->tabWidget->removeTab(currentTab);
    ui->tabWidget->insertTab(currentTab, w,  QString("Mixer %1%2").arg(currentTab+1).arg(name==""? "": ": "+name));
    ui->tabWidget->setCurrentIndex(currentTab);
}

MainWindow* MainWindow::findMainWindow()
{
    for(QWidget* pWidget : QApplication::topLevelWidgets())
    {
        MainWindow *pMainWnd = qobject_cast<MainWindow*>(pWidget);
        if (pMainWnd)
           return pMainWnd;
    }
    return nullptr;
}

