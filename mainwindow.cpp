#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <stdio.h>

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

void MainWindow::InitMixer(QString comPort)
{
    for(int i=0;i<availablePorts.size();i++){
        if(availablePorts[i] == comPort){
            availablePorts.removeAt(i);
        }
    }
    serials[0] = new Serial(comPort, 115200);
    initWidgets[currentTab] = new InitWidget(comPort);
    connect(initWidgets[currentTab], SIGNAL(Start(QString)), this, SLOT(Start(QString)));
    connect(initWidgets[currentTab], SIGNAL(Disconnect(QString)), this, SLOT(Disconnect(QString)));
    switchWidgets(initWidgets[currentTab], comPort);
}

void MainWindow::Start(QString comPort)
{
    activeWidgets[currentTab] = new ActiveWidget(comPort);
//    connect(getSerialPort(), SIGNAL(MessageFinished(QString)), activeWidgets[currentTab], SLOT(recieveMessage(QString)));
    switchWidgets(activeWidgets[currentTab], comPort);
}

void MainWindow::Disconnect(QString portName)
{
    for(int i=0; i<MAX_TABS;i++){
        if(serials[i]){
            if(serials[i]->getSerialPort().portName() == portName){
               serials[i]->disconnect();
               break;
            }
        }
    }
    availablePorts.append(portName);
    startWidgets[currentTab]->Reset();
    switchWidgets(startWidgets[currentTab]);
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

