#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

Dialog::~Dialog()
{
    clearList();
    delete ui;
}

void Dialog::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    emit connClicked(item);
}

void Dialog::on_pushButton_clicked()
{
    emit connClicked(ui->listWidget->selectedItems().at(0));
}

void Dialog::populateList(QList<QSerialPortInfo> a){
    for(int i = 0; i < a.count(); i++){
        QString row = a.at(i).portName() + " " + a.at(i).description();
        ui->listWidget->addItem(row);
    }
}

void Dialog::clearList(){
    ui->listWidget->clear();
}

void Dialog::closeEvent (QCloseEvent *event){
    Q_UNUSED(event);
    clearList();
}
