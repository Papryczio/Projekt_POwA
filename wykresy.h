#ifndef WYKRESY_H
#define WYKRESY_H

#include <QDialog>
#include "qcustomplot.h"
#include <QTimer>
#include <cmath>

namespace Ui {
class Wykresy;
}

class Wykresy : public QDialog
{
    Q_OBJECT

public:
    explicit Wykresy(QWidget *parent = nullptr);
    ~Wykresy();

private slots:
    void graph_1(QList<double> time, QList<double> value, double a);
    void graph_2(QList<double> time, QList<double> value, double a);
    void on_dialSin_valueChanged(int value);

    void on_dialSaw_valueChanged(int value);

signals:
    void sinFreq(int value);
    void sawFreq(int value);

private:
    Ui::Wykresy *ui;
};

#endif // WYKRESY_H
