#ifndef ACTIVEWIDGET_H
#define ACTIVEWIDGET_H

#include <QWidget>
#include "ui_activewidget.h"
#include <QLCDNumber>
#include <QPushButton>
#include <QtCharts>
using namespace QtCharts;
#include <QLineSeries>
#include <QChartView>

QT_BEGIN_NAMESPACE
namespace Ui { class InitWidget; }
QT_END_NAMESPACE

class ActiveWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ActiveWidget(QString, QWidget *parent = nullptr);
    ~ActiveWidget();

signals:


private:
    Ui::ActiveWidget *ui;
    QString comPort;
    QPushButton *stopButton;
    QLCDNumber *speedLCD, *timeLCD;
    QVBoxLayout *chartLayout;
};

#endif // ACTIVEWIDGET_H
