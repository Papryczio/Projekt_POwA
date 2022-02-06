#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include <QSerialPortInfo>
#include <QDebug>
#include <QCloseEvent>
#include <QMessageBox>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_pushButton_clicked();
    void populateList(QList<QSerialPortInfo> a);
    void clearList();

signals:
    void connClicked(QListWidgetItem *item);

private:
    Ui::Dialog *ui;
    void closeEvent(QCloseEvent *close);
};

#endif // DIALOG_H
