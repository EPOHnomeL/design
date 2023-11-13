#include "initwidget.h"
#include <QDebug>
#include <QRegularExpression>>
#include "mainwindow.h"


InitWidget::InitWidget(QString acomPort, QWidget *parent) : QWidget(parent), ui(new Ui::InitWidget)
{
    ui->setupUi(this);
    statusLabel = ui->statusLabel;
    startButton = ui->startButton;
    editProfileButton = ui->editProfileButton;
    createProfileButton = ui->createProfileButton;
    disconnectButton = ui->disconnectButton;
    profileNameEdit = ui->profileNameEdit;
    mixingSpeedEdit = ui->mixingSpeedEdit;
    mixingRatioEdit = ui->mixingRatioEdit;
    mixingTimeEdit = ui->mixingTimeEdit;
    profilesBox = ui->profilesBox;

//    profiles = new QList<Profile>();
    serial = MainWindow::findMainWindow()->getSerialPort();
    comPort = acomPort;
    statusLabel->setText(QString("Status: %1 Connected").arg(acomPort));

    profilesBox->addItem("Select...");
    profilesBox->addItem("Create Profile");
    connect(profilesBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(ProfileSelect(QString)));

    connect(startButton, SIGNAL(clicked()), this, SLOT(StartClicked()));
    connect(disconnectButton, SIGNAL(clicked()), this, SLOT(DisconnectClicked()));
    connect(editProfileButton, SIGNAL(clicked()), this, SLOT(EditClicked()));
    connect(createProfileButton, SIGNAL(clicked()), this, SLOT(CreateClicked()));

    profileNameEdit->setEnabled(false);
    mixingSpeedEdit->setEnabled(false);
    mixingRatioEdit->setEnabled(false);
    mixingTimeEdit->setEnabled(false);
    createProfileButton->setEnabled(false);
    editProfileButton->setEnabled(false);

}

InitWidget::~InitWidget()
{

}

void InitWidget::StartClicked()
{
    emit Start(comPort);
}

void InitWidget::EditClicked()
{
    if(editing){
        bool isInt;
        int speedValue = mixingSpeedEdit->text().toInt(&isInt, 10);
        if(isInt == false)
            qDebug() << "Speed is not an int";

        bool isFloat;
        float timeValue = mixingSpeedEdit->text().toFloat(&isFloat);
        if(isInt == false)
            qDebug() << "Time is not a float";

        QString ratioValue = mixingRatioEdit->text();
        QRegularExpression re("^\\d+:\\d+$");
        if (!re.match(ratioValue).hasMatch())
           qDebug() << "Ratio is not of correct format";

        Profile p = {.name = profileNameEdit->text(), .ratio = ratioValue, .speed = speedValue, .time = timeValue};
        for(int i=2; i<profilesBox->count();i++){
            profilesBox->removeItem(i);
        }

        for(int j=0;j<profiles.count();j++){
            if(prevProfileName == profiles[j].name){
                profiles.removeAt(j);
                profiles.append(p);
            }
        }

        for(int k=0; k<profiles.count();k++){
            profilesBox->addItem(profiles[k].name);
        }
        editing = !editing;
        profileNameEdit->setEnabled(false);
        mixingSpeedEdit->setEnabled(false);
        mixingRatioEdit->setEnabled(false);
        mixingTimeEdit->setEnabled(false);
        editProfileButton->setText("Edit Profile");
        profilesBox->setCurrentIndex(0);
    }else{
        profileNameEdit->setEnabled(true);
        mixingSpeedEdit->setEnabled(true);
        mixingRatioEdit->setEnabled(true);
        mixingTimeEdit->setEnabled(true);
        editProfileButton->setText("Finish Editing Profile");
        prevProfileName = profileNameEdit->text();
        editing=!editing;
    }

}

void InitWidget::CreateClicked()
{
    if(profilesBox->currentIndex() == 1){
        bool isInt;
        int speedValue = mixingSpeedEdit->text().toInt(&isInt, 10);
        if(isInt == false)
            qDebug() << "Speed is not an int";

        bool isFloat;
        float timeValue = mixingSpeedEdit->text().toFloat(&isFloat);
        if(isInt == false)
            qDebug() << "Time is not a float";

        QString ratioValue = mixingRatioEdit->text();
        QRegularExpression re("^\\d+:\\d+$");
        if (!re.match(ratioValue).hasMatch())
           qDebug() << "Ratio is not of correct format";

        Profile p = {.name = profileNameEdit->text(), .ratio = mixingRatioEdit->text(), .speed = speedValue, .time = timeValue};
        profiles.append(p);
        for(int i=2; i<profilesBox->count();i++){
            profilesBox->removeItem(i);
        }
        foreach(auto &profile, profiles){
            profilesBox->addItem(profile.name);
        }
        profilesBox->setCurrentIndex(0);
    }
}

void InitWidget::DisconnectClicked()
{
    emit Disconnect(comPort);
}

void InitWidget::ProfileSelect(QString profileName)
{
    switch(profilesBox->currentIndex()){
    case 0:
        profileNameEdit->setEnabled(false);
        mixingSpeedEdit->setEnabled(false);
        mixingRatioEdit->setEnabled(false);
        mixingTimeEdit->setEnabled(false);
        profileNameEdit->clear();
        mixingSpeedEdit->clear();
        mixingRatioEdit->clear();
        mixingTimeEdit->clear();
        createProfileButton->setEnabled(false);
        editProfileButton->setEnabled(false);
        return;
    case 1:
        profileNameEdit->setEnabled(true);
        mixingSpeedEdit->setEnabled(true);
        mixingRatioEdit->setEnabled(true);
        mixingTimeEdit->setEnabled(true);
        profileNameEdit->clear();
        mixingSpeedEdit->clear();
        mixingRatioEdit->clear();
        mixingTimeEdit->clear();
        createProfileButton->setEnabled(true);
        editProfileButton->setEnabled(false);
        return;
    default:
        profileNameEdit->setEnabled(false);
        mixingSpeedEdit->setEnabled(false);
        mixingRatioEdit->setEnabled(false);
        mixingTimeEdit->setEnabled(false);
        createProfileButton->setEnabled(false);
        editProfileButton->setEnabled(true);
    }

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
