#include "startwidget.h"
#include <QDebug>



StartWidget::StartWidget(QWidget *parent) : QWidget(parent), ui(new Ui::StartWidget)
{
    ui->setupUi(this);
    comPorts = ui->comPorts;

    // Ports
    QList<QSerialPortInfo> ports = info.availablePorts();
    foreach(auto &x,ports)
        comPorts->addItem(x.portName());
    if(ports.length() == 0){
        qDebug() << "Please Insert RS485 usb stick!";
//        exit(EXIT_FAILURE);
    }
//    setupSerial(ports[1].portName(), 256000);

}

StartWidget::~StartWidget()
{

}
