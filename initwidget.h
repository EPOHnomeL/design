#ifndef INITWIDGET_H
#define INITWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include "ui_initwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class InitWidget; }
QT_END_NAMESPACE

class InitWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InitWidget(QString comPort, QWidget *parent = nullptr);
    ~InitWidget();

public slots:
    void StartClicked();
    void EditClicked();
    void CreateClicked();
    void DisconnectClicked();
    void ProfileSelect(QString);

signals:
    void Start();

private:
    Ui::InitWidget *ui;
    QPushButton *startButton, *editProfileButton, *createProfileButton, *disconnectButton;
    QLineEdit *profileNameEdit, *mixingSpeedEdit, *mixingTimeEdit, *mixingRatioEdit;
    QComboBox *profilesBox;
    QLabel *statusLabel;


};

#endif // INITWIDGET_H
