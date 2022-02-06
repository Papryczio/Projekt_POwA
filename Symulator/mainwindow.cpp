#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->device = new QSerialPort;
    connect(device, SIGNAL(errorOccurred(QSerialPort::SerialPortError)), this, SLOT(onErrorOccurred(QSerialPort::SerialPortError)));
    this->timer = new QTimer;
    StanWejsc=0;
    StanWyjsc=0;
    AC1Wejscie=0;
    AC2Wejscie=0;
    AC3Wejscie=0;
    AC1Wyjscie=0;
    AC2Wyjscie=0;
    AC3Wyjscie=0;
    freqSin_in = 0;
    freqSin_out = 0;
    freqSaw_in = 0;
    freqSaw_out = 0;
    ui->wykres1->addGraph();
    ui->wykres2->addGraph();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete device;
    delete timer;
}


void MainWindow::on_pushButtonSzukaj_clicked()
{

    qDebug() <<"Wcisnieto przycisk Szukaj";
    QList<QSerialPortInfo> devices;
    devices  = QSerialPortInfo::availablePorts();
    for (int i = 0; i < devices.length(); i++){
        qDebug() << devices.at(i).portName() << devices.at(i).description();
        ui->comboBox->addItem(devices.at(i).portName()+ "\t" + devices.at(i).description());
        this->addToLogs("Znaleziono urzadzenie " + devices.at(i).portName()+ "\t" + devices.at(i).description());


    }

}
void MainWindow::addToLogs(QString message){
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    ui->textEdit->append(currentDateTime + "\t" + message);
}

void MainWindow::on_pushButtonPolacz_clicked()
{


    if(ui->comboBox->count() == 0) {
      this->addToLogs("Nie wykryto żadnych urządzeń!");
      return;
    }

    QString portName = ui->comboBox->currentText().split("\t").first();
    //qDebug()<< ui->comboBox->currentText().split("\t").first();
    this->device->setPortName(portName);

    // OTWÓRZ I SKONFIGURUJ PORT:
    if(device->open(QSerialPort::ReadWrite)) {
      this->device->setBaudRate(QSerialPort::Baud9600);
      this->device->setDataBits(QSerialPort::Data8);
      this->device->setParity(QSerialPort::NoParity);
      this->device->setStopBits(QSerialPort::OneStop);
      this->device->setFlowControl(QSerialPort::NoFlowControl);

      this->addToLogs("Otwarto port szeregowy.");
        qDebug() << "Otwarto port szeregowy";
      connect(this->device, SIGNAL(readyRead()), this, SLOT(readFromPort()));
      connect(this->timer, SIGNAL(timeout()),this, SLOT(onTimerInterrupt()));
      timer->start(1000);
      ui->wykres1->addGraph();

    } else {
      this->addToLogs("Otwarcie portu szeregowego się nie powiodło!");
    }

}

void MainWindow::on_pushButtonRozlacz_clicked()
{
    this->timer->stop();
    if(this->device->isOpen()) {
      this->device->close();
      this->addToLogs("Zamknięto połączenie.");
    } else {
      this->addToLogs("Port nie jest otwarty!");
      return;
    }
}

void MainWindow::onErrorOccurred(QSerialPort::SerialPortError error)
{
    qDebug() << error << device->error();
}


void MainWindow::readFromPort() {
  while(this->device->canReadLine()) {
    QString line = this->device->readLine();
    //qDebug() << line;

    QString terminator = "\n";
    int pos = line.lastIndexOf(terminator);
    //qDebug() << line.left(pos);

    this->addToLogs(line.left(pos));
    QString str= line.left(pos);
    if(str.contains("OUT=")){
        QString dane = str.split("=").last();
        if(!dane.isEmpty()){
           QList<QString> lista = dane.split(",");
           if(lista.length() == 6){
               int l1=lista.at(0).toInt();
               if(l1 >= 0 && l1 <256){
                   StanWyjsc = 0;
                   unsigned char pom = l1;
                   if(pom&0x80){                // NAPRAWIONY BUG
                       StanWyjsc = StanWyjsc+128;
                       ui->label7->setText("1");
                   }
                   else ui->label7->setText("0");

                   if(pom&0x40){
                       StanWyjsc = StanWyjsc+64;
                       ui->label6->setText("1");
                   }
                   else ui->label6->setText("0");
                   if(pom&0x20){
                       StanWyjsc = StanWyjsc+32;
                       ui->label5->setText("1");
                   }
                   else ui->label5->setText("0");
                   if(pom&0x10){
                       StanWyjsc = StanWyjsc+16;
                       ui->label4->setText("1");
                   }
                   else ui->label4->setText("0");
                   if(pom&0x08){
                       StanWyjsc = StanWyjsc+8;
                       ui->label3->setText("1");
                   }
                   else ui->label3->setText("0");
                   if(pom&0x04){
                       StanWyjsc = StanWyjsc+4;
                       ui->label2->setText("1");
                   }
                   else ui->label2->setText("0");
                   if(pom&0x02){
                       StanWyjsc = StanWyjsc+2;
                       ui->label1->setText("1");
                   }
                   else ui->label1->setText("0");
                   if(pom&0x01){
                       StanWyjsc = StanWyjsc+1;
                       ui->label0->setText("1");
                   }
                   else ui->label0->setText("0");

                   ui->StanWyjsc->setText(QString::number(StanWyjsc));
               }
               freqSin_in = lista.at(4).toDouble();
               freqSaw_in = lista.at(5).toDouble();

               int l2 = lista.at(1).toInt();
               if(l2 >= 0 && l2 < 1024){
                   l2 =(int) ((l2*100)/1023);
                   ui->AC1->setValue(l2);
               }
               int l3 = lista.at(2).toInt();
               if(l3 >= 0 && l3 < 1024){
                   l3 = (int) ((l3*100)/1023);
                   ui->AC2->setValue(l3);
               }
               int l4 = lista.at(3).toInt();
               if(l4 >= 0 && l4 < 1024){
                   l4 = (int) ((l4*100)/1023);
                   ui->AC3->setValue(l4);
               }
           }
        }


    }
  }
}

void MainWindow::onTimerInterrupt(){
    double val = 0;
    double val2 = 0;
    static int zegar = 0;
    static double czas = 0;
    static QList<double> time;
    static QList<double> value;
    static QList<double> value2;
    zegar++;
    czas=czas+0.1;
    val = sin(freqSin_in*2*M_PI*czas);
    val2 = abs(fmod(freqSaw_in*10*czas, 10) - 5);
    time.append(czas);
    value.append(val);
    value2.append(val2);
    ui->wykres1->graph(0)->setData(time.toVector(),value.toVector());
    ui->wykres2->graph(0)->setData(time.toVector(), value2.toVector());
    ui->wykres1->xAxis->setRange(0, 10);
    ui->wykres1->yAxis->setRange(-2, 2);
    ui->wykres2->xAxis->setRange(0, 10);
    ui->wykres2->yAxis->setRange(-1, 6);
    ui->wykres1->replot();
    ui->wykres2->replot();
    AC1Wyjscie = ui->dial->value();
    AC2Wyjscie = ui->dial_2->value();
    AC3Wyjscie = ui->dial_3->value();
    QString str = "IN=";
    str=str + QString::number(StanWejsc)+","+ QString::number(AC1Wyjscie)+","+ QString::number(AC2Wyjscie)+","+ QString::number(AC3Wyjscie)+ "," +
            QString::number(freqSin_out) + "," + QString::number(freqSaw_out) +"\n";
    this->device->write(str.toLocal8Bit());
    //qDebug() << zegar;


}


void MainWindow::on_checkBox_stateChanged(int arg1)
{

    if(arg1 == 2){
        StanWejsc=StanWejsc+1;
    }
    else StanWejsc=StanWejsc-1;;
    qDebug()<<StanWejsc;

    ui->StanWejsc->setText(QString::number(StanWejsc));
}

void MainWindow::on_checkBox_2_stateChanged(int arg1)
{
    qDebug()<< arg1;
    if(arg1 == 2){
        StanWejsc=StanWejsc+2;
    }
    else StanWejsc=StanWejsc-2;

    ui->StanWejsc->setText(QString::number(StanWejsc));

}

void MainWindow::on_checkBox_3_stateChanged(int arg1)
{
    if(arg1 == 2){
        StanWejsc=StanWejsc+4;
    }
    else StanWejsc=StanWejsc-4;

    ui->StanWejsc->setText(QString::number(StanWejsc));

}

void MainWindow::on_checkBox_4_stateChanged(int arg1)
{    if(arg1 == 2){
        StanWejsc=StanWejsc+8;
    }
    else StanWejsc=StanWejsc-8;

    ui->StanWejsc->setText(QString::number(StanWejsc));

}

void MainWindow::on_checkBox_8_stateChanged(int arg1)
{
    if(arg1 == 2){
        StanWejsc=StanWejsc+16;
    }
    else StanWejsc=StanWejsc-16;

    ui->StanWejsc->setText(QString::number(StanWejsc));

}

void MainWindow::on_checkBox_5_stateChanged(int arg1)
{
    if(arg1 == 2){
        StanWejsc=StanWejsc+32;
    }
    else StanWejsc=StanWejsc-32;

    ui->StanWejsc->setText(QString::number(StanWejsc));
}

void MainWindow::on_checkBox_6_stateChanged(int arg1)
{
    if(arg1 == 2){
        StanWejsc=StanWejsc+64;
    }
    else StanWejsc=StanWejsc-64;

    ui->StanWejsc->setText(QString::number(StanWejsc));

}

void MainWindow::on_checkBox_7_stateChanged(int arg1)
{
    if(arg1 == 2){
        StanWejsc=StanWejsc+128;
    }
    else StanWejsc=StanWejsc-128;

    ui->StanWejsc->setText(QString::number(StanWejsc));

}

void MainWindow::on_dial_4_valueChanged(int value)
{
    freqSin_out = (double)value/10;
}

void MainWindow::on_dial_5_valueChanged(int value)
{
    freqSaw_out = (double)value/10;
}
