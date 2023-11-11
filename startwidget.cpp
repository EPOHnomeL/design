#include "startwidget.h"
#include <QDebug>



StartWidget::StartWidget(QWidget *parent) : QWidget(parent), ui(new Ui::StartWidget)
{
    ui->setupUi(this);
    comPorts = ui->comPorts;

    connect(comPorts, SIGNAL(currentIndexChanged(QString)), this, SLOT(currentChanged(QString)));

    QList<QSerialPortInfo> ports = info.availablePorts();
    if(ports.length() == 0){
        comPorts->addItem("Please Insert RS485 usb stick!");
    } else{
        comPorts->addItem("Select...");
    }
    foreach(auto &x,ports)
        comPorts->addItem(x.portName());
}

StartWidget::~StartWidget()
{

}

void StartWidget::currentChanged(QString s )
{
    if(comPorts->currentIndex() != 0){
        emit comPortSelected(s);
    }
}
