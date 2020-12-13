#include "connector.h"
#include <QTextStream>

int Connector::getEi() const
{
    return m_ei;
}

int Connector::getEo() const
{
    return m_eo;
}

int Connector::getPos() const
{
    return m_pos;
}

QString Connector::toString()
{
    QString res;
    QTextStream buf(&res);
    buf<<"from : "<<m_ei<<", to : "<<m_eo<<", pos : "<<m_pos;
    return res;
}

bool Connector::lessPositionThan(Connector conn_1, Connector conn_2)
{
    return conn_1.getPos() < conn_2.getPos();
}

bool Connector::lessDepuisThan(Connector conn_1, Connector conn_2)
{
    return conn_1.getEi() < conn_2.getEi();
}

void Connector::setPos(int pos)
{
    m_pos = pos;
}

Connector::Connector(int ei, int eo, int pos): m_ei(ei), m_eo(eo), m_pos(pos)
{

}
