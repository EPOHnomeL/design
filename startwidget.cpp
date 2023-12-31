#include "startwidget.h"
#include "mainwindow.h"
#include <QDebug>



StartWidget::StartWidget(QWidget *parent) : QWidget(parent), ui(new Ui::StartWidget)
{
    ui->setupUi(this);
    comPorts = ui->comPorts;
    connect(comPorts, SIGNAL(currentTextChanged(QString)), this, SLOT(CurrentChanged(QString)));
    QList<QString> ports =  MainWindow::findMainWindow()->getAvailablePorts();
    if(ports.length() == 0){
        comPorts->addItem("No mixer found");
    } else{
        if(comPorts->count() == 0){
            comPorts->addItem("Select...");
        }
    }
    for(int i=1; i<comPorts->count();i++){
        comPorts->removeItem(i);
    }
    foreach(auto &x,ports)
        comPorts->addItem(x);
    comPorts->setCurrentIndex(0);
}

StartWidget::~StartWidget()
{

}

void StartWidget::Reset()
{
    QList<QString> ports =  MainWindow::findMainWindow()->getAvailablePorts();
    if(ports.length() == 0){
        comPorts->setItemText(0, "No mixer found");
    } else{
        if(comPorts->count() == 0){
            comPorts->setItemText(0, "Select...");
        }
    }
    for(int i=1; i<comPorts->count();i++){
        comPorts->removeItem(i);
    }
    foreach(auto &x,ports){
        bool b = true;
        for(int j=0;j<comPorts->count();j++){
            if(comPorts->itemText(j) == x)
                b = false;
        }
        if(b)
            comPorts->addItem(x);
    }
    comPorts->setCurrentIndex(0);
}

void StartWidget::CurrentChanged(QString s )
{
    if(s!= "No mixer found" && s!="Select..."){
        emit ComPortSelected(s);
    }
}

QComboBox *StartWidget::getComPorts() const
{
    return comPorts;
}
