#ifndef LED_H
#define LED_H

#include <QWidget>

class LED: public QWidget {
    Q_OBJECT
  public:
    LED(QWidget *parent = 0);
    void setState(bool state);
  public slots:
  protected:
    void paintEvent(QPaintEvent *);
  private:
    bool lit;
    QColor OnColor;
    QColor OffColor;
    int Size;
};

#endif // LED_H
