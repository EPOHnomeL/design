#include "initwidget.h"
#include <QDebug>
#include <QRegularExpression>
#include "mainwindow.h"


InitWidget::InitWidget(QString acomPort, QWidget *parent) : QWidget(parent), ui(new Ui::InitWidget)
{
    ui->setupUi(this);
    statusLabel = ui->statusLabel;
    disconnectButton = ui->disconnectButton;
    e_profId = ui->e_profId;
    e_motorRPM = ui->e_motorRPM;
    e_bucketRPM = ui->e_bucketRPM;
    e_waterPercentage = ui->e_waterPercentage;
    e_armAngle = ui->e_armAngle;
    e_time = ui->e_time;
    e_motorDirection = ui->e_motorDirection;
    e_bucketDirection = ui->e_bucketDirection;
    profilesBox = ui->profilesBox;
    profilesBox->addItem("Select...");

    serial = MainWindow::findMainWindow()->getSerialPort();
    comPort = acomPort;
    statusLabel->setText(QString("Status: %1 Connected; Waiting for Operator to select profile").arg(acomPort));
    connect(profilesBox, SIGNAL(currentTextChanged(QString)), this, SLOT(ProfileSelect(QString)));
    connect(disconnectButton, SIGNAL(clicked()), this, SLOT(DisconnectClicked()));

    e_profId->setEnabled(false);
    e_motorRPM->setEnabled(false);
    e_bucketRPM->setEnabled(false);
    e_waterPercentage->setEnabled(false);
    e_armAngle->setEnabled(false);
    e_time->setEnabled(false);
    e_motorDirection->setEnabled(false);
    e_bucketDirection->setEnabled(false);

    QList<quint16> list = {1, 50, 300, 40, 90, 600, 1, 0, 2, 80, 500, 200, 60, 1000, 0, 1, 3, 20, 800, 200, 95, 200, 1, 1};
    refreshProfiles(list);
}

void InitWidget::refreshProfiles(QList<quint16> profileData)
{
    profiles.clear();
    for(int i=0;i<3;i++){
        Profile p = {.profileID = profileData.at(0+(i*8)),
                     .waterPercentage=profileData.at(1+(i*8)),
                     .motorRPM=profileData.at(2+(i*8)),
                     .bucketRPM=profileData.at(3+(i*8)),
                     .armAngle=profileData.at(4+(i*8)),
                     .time=profileData.at(5+(i*8)),
                     .motorRotation= (profileData.at(6+(i*8)) == 1),
                     .bucketRotation = (profileData.at(7+(i*8)) == 1)};
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

    e_profId->setText(QString("Profile %1").arg(p.profileID));
    e_motorRPM->setText(QString("%1").arg(p.motorRPM));
    e_bucketRPM->setText(QString("%1").arg(p.bucketRPM));
    e_waterPercentage->setText(QString("%1").arg(p.waterPercentage));
    e_armAngle->setText(QString("%1").arg(p.armAngle));
    e_time->setText(QString("%1").arg(p.time));
    e_motorDirection->setText(QString("%1").arg(p.motorRotation ? "Anti-Clockwise" : "Clockwise"));
    e_bucketDirection->setText(QString("%1").arg(p.bucketRotation ? "Anti-Clockwise" : "Clockwise"));
}
