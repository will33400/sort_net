#include "mycanvas.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPainter>
#include <QTextStream>

MyCanvas::MyCanvas(QWidget *parent) : QWidget(parent)
{

}

void MyCanvas::SortnetInit(int nb)
{
    m_sortnet = new Sortnet(nb);
    m_nb_conn = nb;
}

void MyCanvas::SortnetAppendConnector(int ei, int eo)
{
    m_sortnet->AppendConnector(ei, eo);
}

void MyCanvas::SortnetInject(QList<int> values, int size)
{
    m_sortnet->inject(values, size);
    m_nb_digit = size;

    for(int i = 0; i < m_nb_digit; i++) {
        QPointF *temp = new QPointF(0, 0);
        m_digit_Coo.append(temp);
    }
}

int MyCanvas::SortnetPop(int index)
{
    return m_sortnet->pop(index);
}

void MyCanvas::SortnetParallel()
{
    m_sortnet->parallel();
}

void MyCanvas::paintEvent(QPaintEvent *)
{
    changeScale();

    QPainter painter(this);
    char digit_text[3];

    QPen black_slim(Qt::black,1);
    QPen black_fat(Qt::black,3);

    painter.setPen(black_slim);

    //display digits lines + input digits

    for(int i = 0; i < m_nb_digit; i++) {
        painter.drawLine(
            changeCooDigit(QPointF(0, i)),
            changeCooDigit(QPointF(m_nb_conn, i))
        );

        sprintf(digit_text,"%d",m_sortnet->pop(i));
        QString text(digit_text);

        painter.drawText(
            changeCooInDigitText(QPointF(0, i)),
            text
        );
    }

    //display connectors

    for(int i = 0; i < m_nb_conn; i++) {
        painter.drawLine(
            changeCooConn(QPointF(m_sortnet->at(i)->getPos(), m_sortnet->at(i)->getEi())),
            changeCooConn(QPointF(m_sortnet->at(i)->getPos(), m_sortnet->at(i)->getEo()))
        );
        painter.setPen(black_fat);
        painter.drawPoint(changeCooConn(QPointF(m_sortnet->at(i)->getPos(), m_sortnet->at(i)->getEi())));
        painter.drawPoint(changeCooConn(QPointF(m_sortnet->at(i)->getPos(), m_sortnet->at(i)->getEo())));
        painter.setPen(black_slim);
    }

    //display sorting

    for (int i = 0; i < m_nb_digit; i++) {
        //QPoint temp_1 = changeCooSorting(QPointF(0, i), i);
        painter.drawLine(
            changeCooSorting(QPointF(0, i), i),
            changeCooSorting(QPointF(1, i), i)
        );
        // get last coo for each digit
        m_digit_Coo.at(i)->setX(changeCooSorting(QPointF(1, i), i).x());
        m_digit_Coo.at(i)->setY(changeCooSorting(QPointF(1, i), i).y());
    }

    for(int i = 0; i < m_nb_conn; i++) {

        if(
                m_sortnet->pop(m_sortnet->getConnector(i).getEi()) >
                m_sortnet->pop(m_sortnet->getConnector(i).getEo())
          ) {

            //get into dist the offset between the two digits

            int dist = m_sortnet->getConnector(i).getEo() - m_sortnet->getConnector(i).getEi();
            qDebug()<<dist<<m_sortnet->getConnector(i).toString();

            //exchange the two values

            int temp = m_sortnet->pop(m_sortnet->getConnector(i).getEi());
            m_sortnet->replace(m_sortnet->getConnector(i).getEi(), m_sortnet->pop(m_sortnet->getConnector(i).getEo()));
            m_sortnet->replace(m_sortnet->getConnector(i).getEo(), temp);
            qDebug()<<m_sortnet->toString();

            //display vertical lines for permutation

            painter.drawLine(
                *m_digit_Coo.at(m_sortnet->getConnector(i).getEi()),
                QPointF(
                            m_digit_Coo.at(m_sortnet->getConnector(i).getEi())->rx(),
                            m_digit_Coo.at(m_sortnet->getConnector(i).getEi())->ry() + (scale_y * dist)
                       )
            );

            painter.drawLine(
                *m_digit_Coo.at(m_sortnet->getConnector(i).getEo()),
                QPointF(
                            m_digit_Coo.at(m_sortnet->getConnector(i).getEo())->rx(),
                            m_digit_Coo.at(m_sortnet->getConnector(i).getEo())->ry() - (scale_y * dist)
                       )
            );

            //update last point position

            m_digit_Coo.at(m_sortnet->getConnector(i).getEi())->setY(
                        m_digit_Coo.at(
                            m_sortnet->getConnector(i).getEi())->ry() + (scale_y * dist));

            m_digit_Coo.at(m_sortnet->getConnector(i).getEo())->setY(
                        m_digit_Coo.at(
                            m_sortnet->getConnector(i).getEo())->ry() - (scale_y * dist));
        }

        //permute points indexes into list

        QPointF *temp = m_digit_Coo.at(m_sortnet->getConnector(i).getEi());
        m_digit_Coo.replace(m_sortnet->getConnector(i).getEi(), m_digit_Coo.at(m_sortnet->getConnector(i).getEo()));
        m_digit_Coo.replace(m_sortnet->getConnector(i).getEo(), temp);

        for (int j = 0; j < m_nb_digit; j++) {

            //default : display horizontal line for each digit from last pos to new pos (conn) then update coo

            painter.drawLine(
                *m_digit_Coo.at(j),
                QPointF(m_digit_Coo.at(j)->rx() + scale_x, m_digit_Coo.at(j)->ry())
            );

            m_digit_Coo.at(j)->setX(m_digit_Coo.at(j)->rx() + scale_x);
        }

    }

    //display output digits

    for(int i = 0; i < m_nb_digit; i++) {

        sprintf(digit_text,"%d",m_sortnet->pop(i));
        QString text(digit_text);

        painter.drawText(
            changeCooOutDigitText(QPointF(m_nb_conn, i)),
            text
        );
    }
}

void MyCanvas::changeScale()
{
    //get canvas size
    int widthCan = size().width();
    int heightCan = size().height();
    //recompute scale_x, scale_y
    scale_x = widthCan / m_nb_conn;
    scale_y = heightCan / m_nb_digit;
    qDebug()<<scale_x<<", "<<scale_y;
}

QPointF MyCanvas::changeCooDigit(QPointF p)
{
    return QPointF(p.rx()*scale_x, p.ry()*scale_y + scale_y/2);
}

QPointF MyCanvas::changeCooInDigitText(QPointF p)
{
    return QPointF(p.rx()*scale_x, p.ry()*scale_y + scale_y/2 +12);
}

QPointF MyCanvas::changeCooOutDigitText(QPointF p)
{
    return QPointF(p.rx()*scale_x - 12, p.ry()*scale_y + scale_y/2 +12);
}

QPointF MyCanvas::changeCooConn(QPointF p)
{
    return QPointF((p.rx() - 1)*scale_x + scale_x/2, p.ry()*scale_y + scale_y/2);
}

QPointF MyCanvas::changeCooSorting(QPointF p, int index)
{
    return QPointF(
        p.rx()*scale_x + (3 * index + 2) - scale_x/2,
        p.ry()*scale_y - (3 * index + 2) + scale_y/2
    );
}

