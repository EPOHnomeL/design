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
    img_mr = ui->img_mr;
    img_br = ui->img_br;

    // Load and set an image at runtime
    QPixmap br(":/rot/Anti-Clockwise.jpg");
    QPixmap scaledPixmap = br.scaledToHeight(img_mr->maximumHeight(), Qt::SmoothTransformation);
    ui->img_br->setPixmap(scaledPixmap);
    ui->img_mr->setPixmap(scaledPixmap);

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
    prof1 = {1, 0 ,0, 0 ,0 ,0 ,0 ,0};
    prof2 = {2, 0 ,0, 0 ,0 ,0 ,0 ,0};
    prof3 = {3, 0 ,0, 0 ,0 ,0 ,0 ,0};
    refreshProfiles(prof1, prof2, prof3);
}

void InitWidget::refreshProfiles(QList<quint16> aprof1,QList<quint16> aprof2,QList<quint16> aprof3)
{
    prof1 = aprof1;
    prof2 = aprof2;
    prof3 = aprof3;
    profiles.clear();
    Profile p1 = {.profileID = aprof1.at(0),
                 .waterPercentage=aprof1.at(1),
                 .motorRPM=aprof1.at(2),
                 .bucketRPM=aprof1.at(3),
                 .armAngle=aprof1.at(4),
                 .time=aprof1.at(5),
                 .motorRotation= (aprof1.at(7) == 1),
                 .bucketRotation = (aprof1.at(6) == 1)};
    profiles.append(p1);
    if(profilesBox->count() < 4){
        profilesBox->addItem(QString("Profile %1").arg(p1.profileID));
    }

    Profile p2 = {.profileID = aprof2.at(0),
                 .waterPercentage=aprof2.at(1),
                 .motorRPM=aprof2.at(2),
                 .bucketRPM=aprof2.at(3),
                 .armAngle=aprof2.at(4),
                 .time=aprof2.at(5),
                 .motorRotation= (aprof2.at(7) == 1),
                 .bucketRotation = (aprof2.at(6) == 1)};
    profiles.append(p2);
    if(profilesBox->count() < 4){
        profilesBox->addItem(QString("Profile %1").arg(p2.profileID));
    }

    Profile p3 = {.profileID = aprof3.at(0),
                 .waterPercentage=aprof3.at(1),
                 .motorRPM=aprof3.at(2),
                 .bucketRPM=aprof3.at(3),
                 .armAngle=aprof3.at(4),
                 .time=aprof3.at(5),
                 .motorRotation= (aprof3.at(7) == 1),
                 .bucketRotation = (aprof3.at(6) == 1)};
    profiles.append(p3);
    if(profilesBox->count() < 4){
        profilesBox->addItem(QString("Profile %1").arg(p3.profileID));
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
    if(s == "Select..."){
        e_profId->setText("");
        e_motorRPM->setText("");
        e_bucketRPM->setText("");
        e_waterPercentage->setText("");
        e_armAngle->setText("");
        e_time->setText("");
        return;
    }

    QString number = s[8];
    Profile p = {};
    bool recievedData = false;
    foreach(auto &profile, profiles){
        if(profile.profileID == number.toInt()){
            p = profile;
            if(p.waterPercentage != 0){
                recievedData = true;
            }
        }
    }

    if(!recievedData){
        e_profId->setText("Waiting for Data");
        e_motorRPM->setText("");
        e_bucketRPM->setText("");
        e_waterPercentage->setText("");
        e_armAngle->setText("");
        e_time->setText("");
        QPixmap br(":/rot/Anti-Clockwise.jpg");
        QPixmap scaledPixmap = br.scaledToHeight(img_mr->maximumHeight(), Qt::SmoothTransformation);
        ui->img_br->setPixmap(scaledPixmap);
        ui->img_mr->setPixmap(scaledPixmap);
        return;
    }

    e_profId->setText(QString("Profile %1").arg(p.profileID));
    e_motorRPM->setText(QString("%1").arg(p.motorRPM));
    e_bucketRPM->setText(QString("%1").arg(p.bucketRPM));
    e_waterPercentage->setText(QString("%1").arg(p.waterPercentage));
    e_armAngle->setText(QString("%1").arg(p.armAngle));
    e_time->setText(QString("%1").arg(p.time));

    QPixmap br(QString(":/rot/%1.jpg").arg(p.bucketRotation ? "Anti-Clockwise" : "Clockwise"));
    QPixmap scaledPixmap1 = br.scaledToHeight(img_br->maximumHeight(), Qt::SmoothTransformation);
    ui->img_br->setPixmap(scaledPixmap1);

    QPixmap mr(QString(":/rot/%1.jpg").arg(p.motorRotation ? "Anti-Clockwise" : "Clockwise"));
    QPixmap scaledPixmap2 = mr.scaledToHeight(img_mr->maximumHeight(), Qt::SmoothTransformation);
    ui->img_mr->setPixmap(scaledPixmap2);
}

void InitWidget::refreshProfile1(QList<quint16> p)
{
    profilesBox->setCurrentIndex(0);
    refreshProfiles(p, prof2, prof3);
}

void InitWidget::refreshProfile2(QList<quint16> p)
{
    profilesBox->setCurrentIndex(0);
    refreshProfiles(prof1, p, prof3);
}

void InitWidget::refreshProfile3(QList<quint16> p)
{
    profilesBox->setCurrentIndex(0);
    refreshProfiles(prof1, prof2, p);
}

void InitWidget::currentProfileChanged(uint16_t i)
{
    if(i!=0)
        statusLabel->setText(QString("Status: %1 Connected; Profile %2 Selected").arg(comPort).arg(i));
}
