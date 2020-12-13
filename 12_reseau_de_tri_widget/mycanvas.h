#ifndef MYCANVAS_H
#define MYCANVAS_H

#include <QWidget>
#include "sortnet.h"

class MyCanvas : public QWidget
{
    Q_OBJECT
private:
    int nb_cube_x = 10, nb_cube_y = 5;
    int scale_x, scale_y;
    int m_nb_conn;
    int m_nb_digit;

    void changeScale();

    QPointF changeCooDigit(QPointF p);
    QPointF changeCooInDigitText(QPointF p);
    QPointF changeCooOutDigitText(QPointF p);
    QPointF changeCooConn(QPointF p);
    QPointF changeCooSorting(QPointF p, int index);

    Sortnet *m_sortnet;
    QList<QPointF *> m_digit_Coo;
public:
    explicit MyCanvas(QWidget *parent = nullptr);
    void SortnetInit(int nb);
    void SortnetAppendConnector(int ei, int eo);
    void SortnetInject(QList<int> values, int size);
    int SortnetPop(int index);
    void SortnetParallel();

protected:
    void paintEvent(QPaintEvent *);
signals:

public slots:
};

#endif // MYCANVAS_H
