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
    void refreshProfile1(QList<quint16>);
    void refreshProfile2(QList<quint16>);
    void refreshProfile3(QList<quint16>);

signals:
    void Start(QString);
    void Disconnect(QString);

private:
    Ui::InitWidget *ui;
    QList<quint16> prof1;
    QList<quint16> prof2;
    QList<quint16> prof3;
    QPushButton *disconnectButton;
    QLineEdit *e_profId, *e_motorRPM, *e_bucketRPM, *e_waterPercentage, *e_armAngle, *e_time;
    QComboBox *profilesBox;
    QLabel *statusLabel, *img_mr, *img_br;
    QString comPort;
    QList<Profile> profiles;
    bool editing = false;
    QString prevProfileName ="";
    Serial *serial;
    void refreshProfiles(QList<quint16> prof1,QList<quint16> prof2,QList<quint16> prof3);
};

#endif // INITWIDGET_H
