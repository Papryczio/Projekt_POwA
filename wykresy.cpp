#include "wykresy.h"
#include "ui_wykresy.h"

Wykresy::Wykresy(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Wykresy)
{
    ui->setupUi(this);
    ui->widget->addGraph();
    ui->widget->xAxis->setRange(0, 10);
    ui->widget->yAxis->setRange(-2, 2);
    ui->widget_2->addGraph();
    ui->widget_2->xAxis->setRange(0,10);
    ui->widget_2->yAxis->setRange(-1,6);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

Wykresy::~Wykresy()
{
    delete ui;
}

void Wykresy::graph_1(QList<double> time , QList<double> value, double a){
    ui->widget->graph(0)->setData(time.toVector(), value.toVector());
    ui->widget->replot();
    ui->label_6->setText(QString::number(a) + " Hz");
}
void Wykresy::graph_2(QList<double> time, QList<double> value, double a){
    ui->widget_2->graph(0)->setData(time.toVector(), value.toVector());
    ui->widget_2->replot();
    ui->label_8->setText(QString::number(a) + " Hz");
}

void Wykresy::on_dialSin_valueChanged(int value)
{
    emit(sinFreq(value));
    ui->label_5->setText(QString::number((double)value/10) + " Hz");
}

void Wykresy::on_dialSaw_valueChanged(int value)
{
    emit(sawFreq(value));
    ui->label_7->setText(QString::number((double)value/10) + " Hz");
}
