#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QList>
#include <QMessageBox>
#include "dialog.h"
#include "wykresy.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void search();

    void portDisconnect();

    void connectToPort(QListWidgetItem *item);

    void readFromPort();

    void onTimerInterrupt();

    void freqSin_out_setter(int a);
    void freqSaw_out_setter(int a);

    void on_checkBox_stateChanged(int arg1);
    void on_checkBox_2_stateChanged(int arg1);
    void on_checkBox_3_stateChanged(int arg1);
    void on_checkBox_4_stateChanged(int arg1);
    void on_checkBox_5_stateChanged(int arg1);
    void on_checkBox_6_stateChanged(int arg1);
    void on_checkBox_7_stateChanged(int arg1);
    void on_checkBox_8_stateChanged(int arg1);

    void on_dial_valueChanged(int value);
    void on_dial_2_valueChanged(int value);

    void on_dial_3_valueChanged(int value);

    void on_pushButton_3_clicked();

signals:
    void populate(QList<QSerialPortInfo>);
    void clearListSig();
    void graphSignal_1(QList<double> time, QList<double> value, double a);
    void graphSignal_2(QList<double> time, QList<double> value, double a);

private:
    Ui::MainWindow *ui;
    QSerialPort *COMPort;
    Dialog *dial;
    Wykresy *graphs;
    unsigned char digital_in;
    unsigned char digital_out;
    unsigned int analog1_in;
    unsigned int analog1_out;
    unsigned int analog2_in;
    unsigned int analog2_out;
    unsigned int analog3_in;
    unsigned int analog3_out;
    double freqSin_in;
    double freqSin_out;
    double freqSaw_in;
    double freqSaw_out;

    void addToLogs(QString message);
    void sendToPort(QString message);
    QTimer *timer;
};
#endif // MAINWINDOW_H
