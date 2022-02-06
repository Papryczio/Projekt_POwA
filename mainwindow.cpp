#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include "led.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->COMPort = new QSerialPort;
    dial = new Dialog(this);
    graphs = new Wykresy(this);
    connect(dial, SIGNAL(connClicked(QListWidgetItem*)), this, SLOT(connectToPort(QListWidgetItem*)));
    connect(this, SIGNAL(populate(QList<QSerialPortInfo>)), dial, SLOT(populateList(QList<QSerialPortInfo>)));
    connect(this, SIGNAL(clearListSig()), dial, SLOT(clearList()));
    connect(graphs, SIGNAL(sinFreq(int)), this, SLOT(freqSin_out_setter(int)));
    connect(graphs, SIGNAL(sawFreq(int)), this, SLOT(freqSaw_out_setter(int)));
    this->timer = new QTimer;
    digital_in = 0;
    digital_out = 0;
    analog1_in = 0;
    analog1_out = 0;
    analog2_in = 0;
    analog2_out = 0;
    analog3_in = 0;
    analog3_out = 0;
    freqSin_in = 0;
    freqSin_out = 0;
    freqSaw_in = 0;
    freqSaw_out = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete COMPort;
    delete dial;
    delete timer;
}
//wyszukanie portów
void MainWindow::search(){
    QList<QSerialPortInfo> connections;
    connections = QSerialPortInfo::availablePorts();
    if(connections.isEmpty()){
                QMessageBox::warning(this, "Brak portów", "Nie znaleziono portów COM");
    }
    else{
   for(int i = 0; i< connections.count();i++){
        qDebug() << connections.at(i).portName() << connections.at(i).description();
        this->addToLogs("Znalazłem urządzenie:\n" + connections.at(i).portName() + " " + connections.at(i).description());
   }
   emit populate(connections);
    dial->setModal(true);
    dial->show();
    }
}
    void MainWindow::on_pushButton_clicked()
    {
        if(COMPort->isOpen()){
           QMessageBox::StandardButton reply = QMessageBox::question(this,"Ostrzeżenie!", "Przejście do tego panelu spowoduje przerwanie aktualnego połącznia.\nCzy chcesz kontynuować?",QMessageBox::Yes | QMessageBox::No);
           if(reply == QMessageBox::No){
               return;
           }
           else{
               portDisconnect();
               search();
        }
    }
        else{
            search();
        }
    }
//polacznie z portem
    void MainWindow::connectToPort(QListWidgetItem *item){
       if(!COMPort->isOpen()){
           if(COMPort->open(QSerialPort::ReadWrite)){
               QList<QSerialPortInfo> connections = QSerialPortInfo::availablePorts();
               if(connections.isEmpty()){
                  this->addToLogs("Nie wykryto żadnych urządzeń");
                  QMessageBox::warning(this, "Brak portów!", "Nie znaleziono żadnych portów COM.");
                  return;
    } }
          QString portName = item->text().split(" ").first();
          this->COMPort->setPortName(portName);
          if(COMPort->open(QSerialPort::ReadWrite)){
             this->COMPort->setBaudRate(QSerialPort::Baud9600);
             this->COMPort->setDataBits(QSerialPort::Data8);
             this->COMPort->setParity(QSerialPort::NoParity);
             this->COMPort->setStopBits(QSerialPort::OneStop);
             this->COMPort->setFlowControl(QSerialPort::NoFlowControl);

             this->addToLogs("Otwarto port szeregowy");
             emit clearListSig();
             dial->close();
             connect(this->COMPort, SIGNAL(readyRead()), this, SLOT(readFromPort()));
             connect(this->timer, SIGNAL(timeout()),this, SLOT(onTimerInterrupt()));
             this->timer->start(1000);
             ui->pushButton_2->setEnabled(true);

           }  else{
                this->addToLogs("Otwarcie portu szeregowego nie powidło się");
              QMessageBox::warning(this, "Błąd połączenia", "Połączenie z wybranym portem COM nie powiodło się.");
           }
    }   else{
    this->addToLogs("Port już jest otwarty");
    return;
    }

    }
//rozłączenie z portem
    void MainWindow::portDisconnect()
    {
        if(this->COMPort->isOpen()){
            this->COMPort->close();
            this->addToLogs("Zamknięto połącznie");
            this->timer->stop();
        }   else{
            this->addToLogs("Port nie jest otwarty");
            return;
        }
    }
    void MainWindow::on_pushButton_2_clicked()
    {
        portDisconnect();
    }
//dodawanie logu
    void MainWindow::addToLogs(QString message)
    {
       QString currentDataTime = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
       ui->textEdit->append(currentDataTime + "\t" + message);
    }
//odczytywanie z portu
    void MainWindow::readFromPort()
    {
        while(this->COMPort->canReadLine()){
            QString line = this->COMPort->readLine();

            QString terminator = "\n";
            int pos = line.lastIndexOf(terminator);

            this->addToLogs("Odebrano dane:\n" + line.left(pos));
    //przetwarzanie odebranych danych
            QString out = line.left(pos);
            if(out.contains("IN=")){
                QString input = out.split("=").last();
                if(!input.isEmpty()){
                    QList<QString> inputList = input.split(",");
                    if(inputList.length() == 6){
                        int Din = inputList.at(0).toInt();
                        if(Din >= 0 && Din <=255){
                            digital_in = 0;
                            unsigned char in = Din;
                            ui->label_19->setText("Total in: " + QString::number(Din));
                            if(in&0x80){
                                digital_in += 128;
                                LED led;
                                ui->widget->setState(true);
                                qDebug()<<digital_in;
                            }
                            else{
                                ui->widget->setState(false);
                                qDebug()<<digital_in;
                            }
                            if(in&0x40){
                                digital_in += 64;
                                ui->widget_2->setState(true);
                                qDebug()<<digital_in;
                            }
                            else{
                                ui->widget_2->setState(false);
                                qDebug()<<digital_in;
                            }
                            if(in&0x20){
                                digital_in += 32;
                                ui->widget_3->setState(true);
                                qDebug()<<digital_in;
                            }
                            else{
                                ui->widget_3->setState(false);
                                qDebug()<<digital_in;
                            }
                            if(in&0x10){
                                digital_in += 16;
                                ui->widget_4->setState(true);
                                qDebug()<<digital_in;
                            }
                            else{
                                ui->widget_4->setState(false);
                                qDebug()<<digital_in;
                            }
                            if(in&0x08){
                                digital_in += 8;
                                ui->widget_5->setState(true);
                                qDebug()<<digital_in;
                            }
                            else{
                                ui->widget_5->setState(false);
                                qDebug()<<digital_in;
                            }
                            if(in&0x04){
                                digital_in += 4;
                                ui->widget_6->setState(true);
                                qDebug()<<digital_in;
                            }
                            else{
                                ui->widget_6->setState(false);
                                qDebug()<<digital_in;
                            }
                            if(in&0x02){
                                digital_in += 2;
                                ui->widget_7->setState(true);
                                qDebug()<<digital_in;
                            }
                            else{
                                ui->widget_7->setState(false);
                                qDebug()<<digital_in;
                            }
                            if(in&0x01){
                                digital_in += 1;
                                ui->widget_8->setState(true);
                                qDebug()<<digital_in;
                            }
                            else{
                                ui->widget_8->setState(false);
                                qDebug()<<digital_in;
                            }
                        }

                            int Ain1 = inputList.at(1).toInt();
                            if(Ain1 >= 0 && Ain1 < 1024){
                                ui->label_29->setText(QString::number(Ain1) + " / 1023");
                                Ain1 =(int) ((Ain1*100)/1023);
                                ui->progressBar->setValue(Ain1);
                            }
                            int Ain2 = inputList.at(2).toInt();
                            if(Ain2 >= 0 && Ain2 < 1024){
                                ui->label_30->setText(QString::number(Ain2) + " / 1023");
                                Ain2 = (int) ((Ain2*100)/1023);
                                ui->progressBar_2->setValue(Ain2);
                            }
                            int Ain3 = inputList.at(3).toInt();
                            ui->label_31->setText(QString::number(Ain3) + " / 1023");
                            if(Ain3 >= 0 && Ain3 < 1024){
                                Ain3 = (int) ((Ain3*100)/1023);
                                ui->progressBar_3->setValue(Ain3);
                            }

                            freqSin_in = inputList.at(4).toDouble();
                            freqSaw_in = inputList.at(5).toDouble();
                    }
            }
        }
    }
    }
//wysylanie do portu
    void MainWindow::sendToPort(QString message)
    {
        if(this->COMPort->isOpen() && this->COMPort->isWritable()){
            this->addToLogs("Wysłano dane:\n" + message);
            this->COMPort->write(message.toLocal8Bit());
        }   else{
            this->addToLogs("Nie mogę wysłać wiadomości. Port nie jest otwarty.");
        }
    }
//timer
    void MainWindow::onTimerInterrupt(){
       double val_1 = 0;
       double val_2 = 0;
       static double czas = 0;
       static QList<double> time;
       static QList<double> value_1;
       static QList<double> value_2;
       connect(this, SIGNAL(graphSignal_1(QList<double>, QList<double>, double)), graphs, SLOT(graph_1(QList<double>, QList<double>,double)));
       connect(this, SIGNAL(graphSignal_2(QList<double>, QList<double>, double)), graphs, SLOT(graph_2(QList<double>, QList<double>,double)));
       if(graphs->isActiveWindow()){
           if(czas > 10){
               czas = 0;
               time.clear();
               value_1.clear();
               value_2.clear();
           }
       for(int i = 0; i<5; i++){
       time.append(czas);
       czas=czas+0.05;
       val_1 = sin(freqSin_in*2*M_PI*czas);
       val_2 = abs(fmod(freqSaw_in*10*czas, 10) - 5);
       value_1.append(val_1);
       value_2.append(val_2);
       emit graphSignal_1(time, value_1, freqSin_in);
       emit graphSignal_2(time, value_2, freqSaw_in);
}
       }
       QString output = "OUT=";
       output = output + QString::number(digital_out)+ "," + QString::number(analog1_out) + "," + QString::number(analog2_out) + "," + QString::number(analog3_out)
               + "," + QString::number(freqSin_out) + "," + QString::number(freqSaw_out) + "\n";
       sendToPort(output);

    }

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    arg1 == 2 ? digital_out += 1 : digital_out -= 1;
    ui->label_20->setText("Total out: "+ QString::number(digital_out));
}

void MainWindow::on_checkBox_2_stateChanged(int arg1)
{
    arg1 == 2 ? digital_out += 2 : digital_out -= 2;
    ui->label_20->setText("Total out: "+ QString::number(digital_out));
}

void MainWindow::on_checkBox_3_stateChanged(int arg1)
{
    arg1 == 2 ? digital_out += 4 : digital_out -= 4;
    ui->label_20->setText("Total out: "+ QString::number(digital_out));
}

void MainWindow::on_checkBox_4_stateChanged(int arg1)
{
    arg1 == 2 ? digital_out += 8 : digital_out -= 8;
    ui->label_20->setText("Total out: "+ QString::number(digital_out));
}

void MainWindow::on_checkBox_5_stateChanged(int arg1)
{
    arg1 == 2 ? digital_out += 16 : digital_out -= 16;
    ui->label_20->setText("Total out: "+ QString::number(digital_out));
}

void MainWindow::on_checkBox_6_stateChanged(int arg1)
{
    arg1 == 2 ? digital_out += 32 : digital_out -= 32;
    ui->label_20->setText("Total out: "+ QString::number(digital_out));
}

void MainWindow::on_checkBox_7_stateChanged(int arg1)
{
    arg1 == 2 ? digital_out += 64 : digital_out -= 64;
    ui->label_20->setText("Total out: "+ QString::number(digital_out));
}

void MainWindow::on_checkBox_8_stateChanged(int arg1)
{
    arg1 == 2 ? digital_out += 128 : digital_out -= 128;
    ui->label_20->setText("Total out: "+ QString::number(digital_out));
}

void MainWindow::on_dial_valueChanged(int value)
{
    ui->label_32->setText(QString::number(value) + " / 1023");
    analog1_out = value;
}

void MainWindow::on_dial_2_valueChanged(int value)
{
    ui->label_33->setText(QString::number(value) + " / 1023");
    analog2_out = value;
}

void MainWindow::on_dial_3_valueChanged(int value)
{
    ui->label_34->setText(QString::number(value) + " / 1023");
    analog3_out = value;
}

void MainWindow::on_pushButton_3_clicked()
{
    graphs->show();
}

void MainWindow::freqSin_out_setter(int a){
    freqSin_out = (double)a/10;
}
void MainWindow::freqSaw_out_setter(int a){
    freqSaw_out = (double)a/10;
}
