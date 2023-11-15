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
    profilesBox->addItem("Select...");

    serial = MainWindow::findMainWindow()->getSerialPort();
    comPort = acomPort;
    statusLabel->setText(QString("Status: %1 Connected; Waiting for Operator to select profile").arg(acomPort));
    connect(profilesBox, SIGNAL(currentTextChanged(QString)), this, SLOT(ProfileSelect(QString)));
    connect(disconnectButton, SIGNAL(clicked()), this, SLOT(DisconnectClicked()));

    profileNameEdit->setEnabled(false);
    mixingSpeedEdit->setEnabled(false);
    mixingRatioEdit->setEnabled(false);
    mixingTimeEdit->setEnabled(false);

    QList<quint16> list = {1, 2, 3, 4, 2, 6, 7, 8, 3, 10, 11, 12};
    refreshProfiles(list);
}

void InitWidget::refreshProfiles(QList<quint16> profileData)
{
    profiles.clear();
    for(int i=0;i<3;i++){
        Profile p = {.profileID = profileData.at(0+(i*4)), .time=profileData.at(1+(i*4)), .rpm=profileData.at(2+(i*4)), .percentWater=profileData.at(3+(i*4))};
        profiles.append(p);
        if(profilesBox->count() < 4){
            profilesBox->addItem(QString("Profile %1").arg(p.profileID));
        }
    }
}

InitWidget::~InitWidget()
{

}

void InitWidget::DisconnectClicked()
{
    emit Disconnect(comPort);
}

void InitWidget::ProfileSelect(QString s)
{
    if(s == "Select...")
        return;
    QString number = s[8];
    Profile p = {};
    foreach(auto &profile, profiles){
        if(profile.profileID == number.toInt())
            p = profile;
    }

    profileNameEdit->setText(QString("%1").arg(p.profileID));
    mixingSpeedEdit->setText(QString("%1").arg(p.rpm));
    mixingRatioEdit->setText(QString("%1 %").arg(p.percentWater));
    mixingTimeEdit->setText(QString("%1").arg(p.time));
}
