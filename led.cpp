#include "led.h"
#include <QPainter>

LED::LED(QWidget *parent) :
    QWidget(parent)
{
    setFixedSize(30, 30);
    lit = false;
    OnColor=Qt::green;
    OffColor=Qt::red;
    Size=20;
}

void LED::paintEvent(QPaintEvent *) {
  QPainter p(this);
  QRadialGradient g(QPointF(5,5), 5);
  g.setColorAt(0, Qt::white);
  g.setColorAt(1, lit ? OnColor : OffColor);
  p.setBrush(g);
  p.setPen(Qt::black);
  p.drawEllipse(0,0,Size,Size);
}

void LED::setState(bool state)
{
  lit = state;
  repaint();
}
