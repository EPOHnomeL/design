#ifndef STARTWIDGET_H
#define STARTWIDGET_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QComboBox>
#include <QMainWindow>
#include <QFormLayout>
#include "ui_startwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class StartWidget; }
QT_END_NAMESPACE

class StartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StartWidget(QWidget *parent = nullptr);
    ~StartWidget();

public slots:
    void currentChanged(QString);

signals:
    void comPortSelected(QString);

private:
    Ui::StartWidget *ui;
    QComboBox *comPorts;
    QSerialPort serialPort;
    QSerialPortInfo info;

};

#endif // STARTWIDGET_H
