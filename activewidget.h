#ifndef ACTIVEWIDGET_H
#define ACTIVEWIDGET_H

#include <QWidget>
#include "ui_activewidget.h"
#include <QLCDNumber>
#include <QPushButton>
#include <QtCharts>
#include <QLineSeries>
#include <QChartView>
#include "serial.h"
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class InitWidget; }
QT_END_NAMESPACE

class ActiveWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ActiveWidget(QString, QWidget *parent = nullptr);
    ~ActiveWidget();
    void start();

signals:
    void disconnectActive(QString);

private slots:
    void updateMotor(uint16_t);
    void updateBucket(uint16_t);
    void updateArmAngle(uint16_t);
    void updateTime(uint16_t);
    void disconnectThis();
    void updateChart();
    void rotationsChanged(QList<uint16_t>);


private:
    Ui::ActiveWidget *ui;
    QString comPort;
    QLCDNumber *lcd_armAngle, *lcd_time;
    QLabel *statusLabel, *img_mr, *img_br;
    QChart *chart;
    QVBoxLayout *chartLayout;
    QTimer *updateTimer;
    QDateTimeAxis *axisX;
    QValueAxis *axisY;
    QLineSeries *series1, *series2;
    int m, b =0;
    float time = 50.0f;
    Serial *serial;
    QString data;
};

#endif // ACTIVEWIDGET_H
