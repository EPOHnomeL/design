#include "initwidget.h"

InitWidget::InitWidget(QWidget *parent) : QWidget(parent), ui(new Ui::InitWidget)
{
    ui->setupUi(this);
    statusLabel = ui->statusLabel;
    startButton = ui->startButton;
    editProfileButton = ui->editProfileButton;
    createProfileButton = ui->createProfileButton;
    profileNameEdit = ui->profileNameEdit;
    mixingSpeedEdit = ui->mixingSpeedEdit;
    mixingRatioEdit = ui->mixingRatioEdit;
    mixingTimeEdit = ui->mixingTimeEdit;
    profilesBox = ui->profilesBox;

    profilesBox->addItem("Select...");
}

InitWidget::~InitWidget()
{

}
