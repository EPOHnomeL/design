#ifndef INITWIDGET_H
#define INITWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include "ui_initwidget.h"
#include "serial.h"
#include "types.h"

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
    void DisconnectClicked();
    void ProfileSelect(QString);
    void refreshProfiles(QList<quint16>);

signals:
    void Start(QString);
    void Disconnect(QString);

private:
    Ui::InitWidget *ui;
    QPushButton *disconnectButton;
    QLineEdit *profileNameEdit, *mixingSpeedEdit, *mixingTimeEdit, *mixingRatioEdit;
    QComboBox *profilesBox;
    QLabel *statusLabel;
    QString comPort;
    QList<Profile> profiles;
    bool editing = false;
    QString prevProfileName ="";
    Serial *serial;
};

#endif // INITWIDGET_H
