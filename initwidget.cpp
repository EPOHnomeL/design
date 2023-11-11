#include "initwidget.h"

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

    comPort = acomPort;
    statusLabel->setText(QString("Status: %1 Connected").arg(acomPort));

    profilesBox->addItem("Select...");
    connect(profilesBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(ProfileSelect(QString)));

    connect(startButton, SIGNAL(clicked()), this, SLOT(StartClicked()));
    connect(disconnectButton, SIGNAL(clicked()), this, SLOT(DisconnectClicked()));
    connect(editProfileButton, SIGNAL(clicked()), this, SLOT(EditClicked()));
    connect(createProfileButton, SIGNAL(clicked()), this, SLOT(CreateClicked()));
}

InitWidget::~InitWidget()
{

}

void InitWidget::StartClicked()
{

}

void InitWidget::EditClicked()
{

}

void InitWidget::CreateClicked()
{

}

void InitWidget::DisconnectClicked()
{
    emit Disconnect(comPort);
}

void InitWidget::ProfileSelect(QString)
{

}
