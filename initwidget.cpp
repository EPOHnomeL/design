#include "initwidget.h"
#include <QDebug>
#include <QRegularExpression>
#include "mainwindow.h"


InitWidget::InitWidget(QString acomPort, QWidget *parent) : QWidget(parent), ui(new Ui::InitWidget)
{
    ui->setupUi(this);
    statusLabel = ui->statusLabel;
    disconnectButton = ui->disconnectButton;
    profileNameEdit = ui->profileNameEdit;
    mixingSpeedEdit = ui->mixingSpeedEdit;
    mixingRatioEdit = ui->mixingRatioEdit;
    mixingTimeEdit = ui->mixingTimeEdit;
    profilesBox = ui->profilesBox;

    serial = MainWindow::findMainWindow()->getSerialPort();
    comPort = acomPort;
    statusLabel->setText(QString("Status: %1 Connected; Waiting for Operator").arg(acomPort));
    connect(profilesBox, SIGNAL(currentTextChanged(QString)), this, SLOT(ProfileSelect(QString)));
    connect(disconnectButton, SIGNAL(clicked()), this, SLOT(DisconnectClicked()));

    profileNameEdit->setEnabled(false);
    mixingSpeedEdit->setEnabled(false);
    mixingRatioEdit->setEnabled(false);
    mixingTimeEdit->setEnabled(false);

    // get profiles //
}

InitWidget::~InitWidget()
{

}

void InitWidget::DisconnectClicked()
{
    emit Disconnect(comPort);
}

void InitWidget::ProfileSelect(QString profileName)
{
    Profile p = {};
    foreach(auto &profile, profiles){
        if(profile.name == profileName)
            p = profile;
    }

    profileNameEdit->setText(p.name);
    mixingSpeedEdit->setText(QString("%1").arg(p.speed));
    mixingRatioEdit->setText(p.ratio);
    mixingTimeEdit->setText(QString("%1").arg(p.time));
}
