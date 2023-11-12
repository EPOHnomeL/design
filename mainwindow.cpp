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
//     vLayoutRPM = ui->vLayoutRPM;
//     QLineSeries* series = new QLineSeries();
//     *series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);
//     QChart *chart = new QChart();
//     chart->legend()->hide();
//     chart->addSeries(series);
//     chart->createDefaultAxes();
//     chart->setTitle("Simple line chart example");
//     QChartView *chartView = new QChartView(chart);
//     chartView->setRenderHint(QPainter::Antialiasing);
//     vLayoutRPM->addWidget(chartView);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QList<QString> MainWindow::getAvailablePorts()
{
    return availablePorts;
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
        if(QString(typeid(tabWidget->currentWidget()).name()).compare("StartWidget")){
            if(startWidgets[currentTab] && startWidgets[currentTab]->getComPorts()->count() == 1){
                startWidgets[currentTab]->Reset();
            }
        }
    }
}

void MainWindow::InitMixer(QString comPort)
{
    for(int i=0;i<availablePorts.size();i++){
        if(availablePorts[i] == comPort){
            availablePorts.removeAt(i);
        }
    }
    initWidgets[currentTab] = new InitWidget(comPort);
    connect(initWidgets[currentTab], SIGNAL(Start()), this, SLOT(Start()));
    connect(initWidgets[currentTab], SIGNAL(Disconnect(QString)), this, SLOT(Disconnect(QString)));
    switchWidgets(initWidgets[currentTab], comPort);
}

void MainWindow::Start()
{

}

void MainWindow::Disconnect(QString portName)
{
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

