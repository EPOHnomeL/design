#include "startwidget.h"
#include "mainwindow.h"
#include <QDebug>



StartWidget::StartWidget(QWidget *parent) : QWidget(parent), ui(new Ui::StartWidget)
{
    ui->setupUi(this);
    comPorts = ui->comPorts;

    connect(comPorts, SIGNAL(currentIndexChanged(QString)), this, SLOT(CurrentChanged(QString)));

    QList<QSerialPortInfo> ports =  MainWindow::findMainWindow()->getAvailablePorts();
    if(ports.length() == 0){
        comPorts->addItem("No mixer found");
    } else{
        comPorts->addItem("Select...");
    }
    foreach(auto &x,ports)
        comPorts->addItem(x.portName());
}

StartWidget::~StartWidget()
{

}

void StartWidget::CurrentChanged(QString s )
{
    if(comPorts->currentIndex() != 0){
        emit ComPortSelected(s);
    }
}
